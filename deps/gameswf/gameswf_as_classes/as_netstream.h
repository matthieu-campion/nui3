// as_netstream.h	-- Vitaly Alexeev <tishka92@yahoo.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef GAMESWF_NETSTREAM_H
#define GAMESWF_NETSTREAM_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "base/tu_queue.h"
#include "gameswf_video_impl.h"
#include "gameswf_video_base.h"

#if TU_CONFIG_LINK_TO_FFMPEG == 1

#include <ffmpeg/avformat.h>
#include <ffmpeg/swscale.h>

#define VIDEO_QUEUE_SIZE 20

namespace gameswf
{

	void as_global_netstream_ctor(const fn_call& fn);
	void netstream_new(const fn_call& fn);
	void netstream_close(const fn_call& fn);
	void netstream_pause(const fn_call& fn);
	void netstream_play(const fn_call& fn);
	void netstream_seek(const fn_call& fn);
	void netstream_setbuffertime(const fn_call& fn);

	struct av_packet : public ref_counted
	{
		Uint8* m_data;
		int m_size;
		int m_stream_index;
		Sint64 m_dts;

		av_packet(const AVPacket& packet)
		{
			m_stream_index = packet.stream_index;
			m_size = packet.size;
			m_dts = packet.dts;

			m_data = new Uint8[m_size];
			memcpy(m_data, packet.data, m_size);
		}

		~av_packet()
		{
			delete [] m_data;
		}

		int get_stream_index() const
		{
			return m_stream_index;
		}

		Uint8* get_packet() const
		{
			return m_data;
		}

	};

	template<class T>
	struct audio_queue
	{
		tu_mutex m_mutex;
		tu_queue<T> m_queue;

		bool push(const T& val)
		{
			tu_autolock locker(m_mutex);
			if (m_queue.size() < VIDEO_QUEUE_SIZE)
			{
				m_queue.push(val);
				return true;
			}
			return false;
		}

		int size()
		{
			tu_autolock locker(m_mutex);
			return int(m_queue.size());
		}

		const T& front()
		{
			tu_autolock locker(m_mutex);
			return m_queue.front(); 
		}

		void pop()
		{
			tu_autolock locker(m_mutex);
			m_queue.pop();
		}

		void clear()
		{
			tu_autolock locker(m_mutex);
			m_queue.clear();
		}

		inline void lock()
		{
			m_mutex.lock();
		}

		inline void unlock()
		{
			m_mutex.unlock();
		}

	};

	template<class T>
	struct video_queue
	{
		tu_queue<T> m_queue;

		bool push(const T& val)
		{
			if (m_queue.size() < VIDEO_QUEUE_SIZE)
			{
				m_queue.push(val);
				return true;
			}
			return false;
		}

		int size()
		{
			return int(m_queue.size());
		}

		const T& front()
		{
			return m_queue.front(); 
		}

		void pop()
		{
			m_queue.pop();
		}

		void clear()
		{
			m_queue.clear();
		}

	};

	// container for the decoded sound
	struct decoded_sound : public ref_counted
	{
		int m_size;
		Sint16* m_data;
		Uint8* m_ptr;

		decoded_sound(Sint16* sample, int size) :
			m_size(size),
			m_data(sample)
		{
			m_ptr = (Uint8*) sample;
		}

		~decoded_sound()
		{
			// We can delete to original WAV data now (created in cvt())
			free(m_data);
		}

		int size() const
		{
			return m_size;
		}

		void take(Uint8*& stream, int& len)
		{
			int n = imin(m_size, len);
			memcpy(stream, m_ptr, n);
			stream += n;
			m_ptr += n;
			m_size -= n;
			len -= n;
			assert(m_size >= 0 &&	len >= 0);
		}
	};

	struct as_netstream : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_NETSTREAM };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		as_netstream(player* player);
		~as_netstream();

		video_handler* get_video_handler();
		bool decode_audio(av_packet* packet, Sint16** data, int* size);
		bool decode_video(av_packet* packet, Uint8** data, int* width, int* height);
		void run();
		void pause(int mode);
		void seek(double seek_time);
		void setBufferTime();
		void audio_callback(Uint8* stream, int len);
		void close();
		void play(const char* url);
		double time() const;

	private:

		inline double as_double(AVRational time){	return time.num / (double) time.den; }
		void set_status(const char* level, const char* code);
		bool open_stream(const char* url);
		void close_stream();
		bool read_frame();

		AVFormatContext *m_FormatCtx;
		AVFrame* m_frame;

		// video
		AVCodecContext* m_VCodecCtx;
		AVStream* m_video_stream;

		// audio
		AVCodecContext *m_ACodecCtx;
		AVStream* m_audio_stream;
		smart_ptr<decoded_sound> m_sound;

		double m_start_time;
		double m_video_clock;
		double m_current_clock;

		int m_video_index;
		int m_audio_index;

		tu_string m_url;
		volatile bool m_is_alive;
		volatile bool m_break;
		volatile bool m_pause;

		smart_ptr<av_packet> m_unqueued_data;

		// this is used in the decoder & sound threads
		audio_queue< smart_ptr<av_packet> > m_audio_queue;

		// this is used in  the decoder thread only
		video_queue< smart_ptr<av_packet> > m_video_queue;

		tu_thread* m_thread;
		tu_condition m_decoder;
		smart_ptr<video_handler> m_video_handler;
	};

} // end of gameswf namespace

#else	// ffmpeg is not present

#include "gameswf_video_impl.h"

namespace gameswf
{
	void as_global_netstream_ctor(const fn_call& fn);

	struct as_netstream : public as_object
	{
		video_handler* get_video_handler() { return NULL; }
	};

}

#endif

// GAMESWF_NETSTREAM_H
#endif

