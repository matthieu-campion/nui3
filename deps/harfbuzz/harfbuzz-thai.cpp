/*
 * Copyright (C) 2007  Trolltech ASA
 *
 * This is part of HarfBuzz, an OpenType Layout engine library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#if 0
#include <qtextcodec.h>
#include <qlibrary.h>


static void thaiWordBreaks(const QChar *string, const int len, QCharAttributes *attributes)
{
#ifdef QT_NO_TEXTCODEC
    Q_UNUSED(string);
    Q_UNUSED(len);
    Q_UNUSED(attributes);
#else
    typedef int (*th_brk_def)(const char*, int[], int);
    static QTextCodec *thaiCodec = QTextCodec::codecForMib(2259);
    static th_brk_def th_brk = 0;

#ifndef QT_NO_LIBRARY
    /* load libthai dynamically */
    if (!th_brk && thaiCodec) {
        th_brk = (th_brk_def)QLibrary::resolve(QLatin1String("thai"), "th_brk");
        if (!th_brk)
            thaiCodec = 0;
    }
#endif

    if (!th_brk)
        return;

    QByteArray cstr = thaiCodec->fromUnicode(QString(string, len));

    int brp[128];
    int *break_positions = brp;
    int numbreaks = th_brk(cstr.constData(), break_positions, 128);
    if (numbreaks > 128) {
        break_positions = new int[numbreaks];
        numbreaks = th_brk(cstr.data(),break_positions, numbreaks);
    }

    for (int i = 0; i < len - 1; ++i)
        attributes[i].lineBreakType = QCharAttributes::NoBreak;

    for (int i = 0; i < numbreaks; ++i) {
        if (break_positions[i] > 0)
            attributes[break_positions[i]-1].lineBreakType = QCharAttributes::Break;
    }

    if (break_positions != brp)
        delete [] break_positions;
#endif // QT_NO_TEXTCODEC
}


static void thai_attributes( int script, const QString &text, int from, int len, QCharAttributes *attributes )
{
    Q_UNUSED(script);
    Q_ASSERT(script == QUnicodeTables::Thai);
    thaiWordBreaks(text.unicode() + from, len, attributes);
}


#endif
