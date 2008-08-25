
enum jit_x86_addressing_mode
{
	RegisterIndirect = 0,	//Except ESP
	Indexed8BitDisplacement = 1,  // Except ESP
	Indexed32BitDisplacement = 2, // Except ESP
	RegisterDirect = 3
};

enum jit_x86_scale_factor
{
	ScaleFactorOne = 0,
	ScaleFactorTwo = 1,
	ScaleFactorFour = 2,
	ScaleFactorEight = 3
};

struct jit_x86_modrm
{
	jit_register
m_rm:3;		// Source
	jit_register
m_register_opcode:3;  // Dest
	jit_x86_addressing_mode
m_mode:2;

	operator unsigned char()
	{
		return *( unsigned char *)this;
	}
};

struct jit_x86_sib
{
	jit_register
m_base_register:3,
m_index_register:3;
	jit_x86_scale_factor
m_scale:2;

	operator unsigned char()
	{
		return *( unsigned char *)this;
	}
};

void jit_mov_implementation( jit_function & function, const jit_register destination, const jit_register source )
{
	function.push_byte( 0x8b ); //mov Gv, Ev
	jit_x86_modrm modrm;

	modrm.m_register_opcode = destination;
	modrm.m_rm = source;
	modrm.m_mode = RegisterDirect;

	function.push_byte( modrm );
}

void jit_load_implementation( jit_function & function, const jit_register destination, const jit_register_offset_address & address )
{
	function.push_byte( 0x8b ); //mov Gv, Ev
	jit_x86_modrm modrm;

	modrm.m_register_opcode = destination;
	modrm.m_rm = address.m_register;
	if( address.m_offset > 127 || address.m_offset < -128 )
	{
		modrm.m_mode = Indexed32BitDisplacement;
		function.push_byte( modrm );
		function.push_integer( address.m_offset );
	}
	else
	{
		modrm.m_mode = Indexed8BitDisplacement;
		function.push_byte( modrm );
		function.push_byte( address.m_offset );
	}
}

void jit_sub_implementation( jit_function & function, const jit_register destination, const int value )
{

	if( value <= 127 && value >= -128 )
	{
		jit_x86_modrm modrm;

		function.push_byte( 0x83 );
		modrm.m_register_opcode = (jit_register)5;
		modrm.m_mode = RegisterDirect;
		modrm.m_rm = destination;
		function.push_byte( modrm );
		//__asm sub esp, 4;
		function.push_byte( value );
	}
	else
	{
		assert( 0 )
	}
}
