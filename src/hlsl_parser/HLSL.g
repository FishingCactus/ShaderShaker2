//

grammar HLSL;

options {
    backtrack = true;
    language = Cpp;
}

@lexer::traits
{
    #include <sstream>

    class HLSLLexer; class HLSLParser;

    template<class ImplTraits>
    class HLSLUserTraits : public antlr3::CustomTraitsBase<ImplTraits> {};

    class HLSLLexerTraits : public antlr3::Traits< HLSLLexer, HLSLParser, HLSLUserTraits >
    {
        public:

        static int ConvertToInt32( const std::string & type )
        {
            int
                return_value;

            std::istringstream( type ) >> return_value;

            return return_value;
        }
    };

    typedef HLSLLexerTraits HLSLParserTraits;

    extern void ( *read_file_content_callback )( void * content, size_t & size, const char * );

    ANTLR_BEGIN_NAMESPACE()

    template<>
    class FileUtils< TraitsBase< HLSLUserTraits > > : public FileUtils< TraitsBase< CustomTraitsBase > >
    {
    public:
        typedef TraitsBase< HLSLUserTraits > ImplTraits;

        template<typename InputStreamType>
        static ANTLR_UINT32 AntlrRead8Bit(InputStreamType* input, const ANTLR_UINT8* fileName)
        {
            if ( read_file_content_callback )
            {
                size_t
                    length;
                void
                    * content;

                read_file_content_callback( NULL, length, ( const char * ) fileName );

                if ( !length )
                {
                    return ANTLR_FAIL;
                }

                content = InputStreamType::AllocPolicyType::alloc( length );
                read_file_content_callback( content, length, ( const char * ) fileName );

                input->set_data( (unsigned char*) content );
                input->set_sizeBuf( length );

                input->set_isAllocated( true );

                return ANTLR_SUCCESS;
            }
            else
            {
                return FileUtils< TraitsBase< CustomTraitsBase > >::AntlrRead8Bit( input, fileName );
            }
        }
    };

    ANTLR_END_NAMESPACE()

    using std::string;
}

@parser::includes
{
    #include "HLSLLexer.hpp"
    #include <iostream>
	#include <string>
	#include <set>
	#include <algorithm>
}

@parser::members
{
    static bool is_not_rgba( const char value )
    {
        return value != 'r' && value != 'g' && value != 'b' && value != 'a';
    }

    static bool is_not_xyzw( const char value )
    {
        return value < 'w' || value > 'z';
    }

    static bool IsValidSwizzle( const std::string & swizzle )
    {
        return
            swizzle.size() <= 4
            && (
                std::find_if( swizzle.begin(), swizzle.end(), is_not_rgba ) == swizzle.end()
                || std::find_if( swizzle.begin(), swizzle.end(), is_not_xyzw ) == swizzle.end()
                );
    }

    std::set<std::string>
        TypeTable;
}

translation_unit
	: global_declaration* technique* EOF
	;

global_declaration
    : variable_declaration
	| texture_declaration
	| sampler_declaration
	| struct_definition
	| function_declaration
	;

technique
    : TECHNIQUE Name=ID LCURLY pass* RCURLY
    ;

pass
    : PASS Name=ID LCURLY shader_definition* RCURLY
    ;

shader_definition
    : Type=( VERTEX_SHADER|PIXEL_SHADER )
    ASSIGN COMPILE ShaderType=ID
    FunctionName=ID
    LPAREN shader_argument_list RPAREN SEMI
    ;

shader_argument_list
    : ( shader_argument ( COMMA shader_argument )* )?
    ;

shader_argument
    : constant_expression
    | constructor
    ;


// Statements

statement
    : ( lvalue_expression assignment_operator ) => assignment_statement
    | ( lvalue_expression self_modify_operator ) => post_modify_statement
    | variable_declaration
    | pre_modify_statement
    | expression_statement
    | block_statement
    | if_statement
    | iteration_statement
    | jump_statement
    | SEMI
    ;

assignment_statement
    : lvalue_expression assignment_operator expression SEMI
    ;

pre_modify_statement
    : pre_modify_expression SEMI
    ;

pre_modify_expression
    : self_modify_operator lvalue_expression
    ;

post_modify_statement
    : post_modify_expression SEMI
    ;

post_modify_expression
    : lvalue_expression  self_modify_operator
    ;

self_modify_operator
    : PLUSPLUS
    | MINUSMINUS
    ;

block_statement
    : LCURLY (statement)* RCURLY
    ;

expression_statement
    : expression SEMI
    ;

if_statement
    : IF LPAREN expression  RPAREN statement
        ( ELSE IF LPAREN expression  RPAREN statement )*
        ( ELSE statement )?
    ;

iteration_statement
    : WHILE LPAREN expression RPAREN statement
    | FOR LPAREN ( assignment_statement | variable_declaration )
        equality_expression SEMI modify_expression RPAREN statement
    | DO statement WHILE LPAREN expression RPAREN SEMI
    ;

modify_expression
    : (lvalue_expression assignment_operator ) =>
         lvalue_expression  assignment_operator expression
    | pre_modify_expression
    | post_modify_expression
    ;

jump_statement
    : BREAK SEMI
    | CONTINUE SEMI
    | RETURN ( expression  )? SEMI
    | DISCARD SEMI
    ;

lvalue_expression
    : variable_expression ( postfix_suffix )?
    ;

variable_expression
    : ID( LBRACKET expression RBRACKET )?
    ;

expression
    : conditional_expression
    ;

conditional_expression
    : logical_or_expression ( QUESTION expression COLON conditional_expression  )?
    ;

logical_or_expression
    :  logical_and_expression ( OR logical_and_expression  )*
    ;

logical_and_expression
    : inclusive_or_expression ( AND inclusive_or_expression  )*
    ;

inclusive_or_expression
    : exclusive_or_expression ( BITWISE_OR exclusive_or_expression )*
    ;

exclusive_or_expression
  : and_expression ( BITWISE_XOR and_expression)*
  ;

and_expression
    : equality_expression ( BITWISE_AND equality_expression )*
    ;

equality_expression
    : relational_expression (op=(EQUAL|NOT_EQUAL) relational_expression )*
    ;

relational_expression
    : shift_expression ( op=(LT_TOKEN|GT_TOKEN|LTE|GTE) shift_expression )?
    ;

shift_expression
    : additive_expression (op=(BITWISE_SHIFTL|BITWISE_SHIFTR) additive_expression )*
    ;

additive_expression
    : multiplicative_expression ( op=(PLUS|MINUS) multiplicative_expression )*
    ;

multiplicative_expression
    : cast_expression ( op=(MUL|DIV|MOD) cast_expression )*
    ;

cast_expression
    : LPAREN type ( LBRACKET INT RBRACKET )?  RPAREN cast_expression
    | unary_expression
    ;

unary_expression
    : op=(PLUS|MINUS|NOT|BITWISE_NOT) unary_expression
    | postfix_expression
    ;

postfix_expression
    : primary_expression ( postfix_suffix )?
    ;

postfix_suffix
    : DOT swizzle
    | DOT primary_expression  ( postfix_suffix )?
    ;

swizzle
    : ID { IsValidSwizzle( $ID.text ) }?
    ;

assignment_operator
    : ASSIGN
    | MUL_ASSIGN
    | DIV_ASSIGN
    | ADD_ASSIGN
    | SUB_ASSIGN
    | BITWISE_AND_ASSIGN
    | BITWISE_OR_ASSIGN
    | BITWISE_XOR_ASSIGN
    | BITWISE_SHIFTL_ASSIGN
    | BITWISE_SHIFTR_ASSIGN
    ;

primary_expression
    : constructor
    | call_expression
    | variable_expression
    | literal_value
    | LPAREN expression RPAREN
    ;

constructor
    : type LPAREN argument_expression_list RPAREN
    ;

call_expression
    : ID LPAREN argument_expression_list RPAREN
    ;

argument_expression_list
    : ( expression ( COMMA expression  )*  )?
    ;

// Function

function_declaration
    : storage_class* ( PRECISE )?
        ( type | VOID_TOKEN ) ID LPAREN ( argument_list )? RPAREN
        ( COLON semantic )?
        LCURLY
            ( statement  )*
        RCURLY
	;

argument_list
    : argument  ( COMMA argument  )*
    ;

argument
    : input_modifier? ( type_modifier )? type
        Name=ID
        ( COLON semantic )?
        ( INTERPOLATION_MODIFIER )? ( ASSIGN initial_value  )?
    ;

input_modifier
    : modifier=( IN_TOKEN | OUT_TOKEN | INOUT | UNIFORM )
    ;

// Texture & sampler

texture_type
    :
    TEXTURE
    | TEXTURE1D
    | TEXTURE1DARRAY
    | TEXTURE2D
    | TEXTURE2DARRAY
    | TEXTURE3D
    | TEXTURECUBE
    ;

texture_declaration
    : t=texture_type ID
    ( COLON semantic ) ?
    ( annotations ) ?
    SEMI
    ;

sampler_declaration
    : t=SAMPLER_TYPE
        Name=ID ( ASSIGN SAMPLER_TYPE )? LCURLY (sampler_body)* RCURLY SEMI
    ;

sampler_body
    : TEXTURE ASSIGN LT_TOKEN ID GT_TOKEN SEMI
    | Name=ID ASSIGN Value=ID SEMI
    ;

// Variables

variable_declaration
    : (storage_class )* ( type_modifier )* type variable_declaration_body ( COMMA variable_declaration_body )* SEMI
	;

variable_declaration_body
    : ID ( LBRACKET INT RBRACKET )?
        ( COLON semantic ) ?
        ( COLON packoffset )?
        ( COLON register_rule ) ?
        ( annotations ) ?
        ( ASSIGN initial_value ) ?
    ;

storage_class
    : EXTERN
    | NOINTERPOLATION
    | PRECISE
    | SHARED
    | GROUPSHARED
    | STATIC
    | UNIFORM
    | VOLATILE
    ;

type_modifier
    : 'const'
    | 'row_major'
    | 'column_major'
    ;

packoffset
    :;

register_rule
    :;

annotations
    : LT_TOKEN annotation_entry* GT_TOKEN
    ;

annotation_entry
    :
    Type=( STRING_TYPE | SCALAR_TYPE ) ID
    ASSIGN ( STRING | literal_value  ) SEMI
    ;

initial_value
    :
    expression
    | LCURLY expression  ( COMMA expression  )* RCURLY
    ;

type
    : ( intrinsic_type | user_defined_type | SAMPLER_TYPE )
    ;

intrinsic_type
    : MATRIX_TYPE
    | VECTOR_TYPE
    | SCALAR_TYPE
    ;

user_defined_type // :TODO: validates that it's a valid type
    : ID  { TypeTable.find( $ID.text) != TypeTable.end() }? =>
    ;

struct_definition
    : STRUCT Name=ID { TypeTable.insert( $Name.text ); }
    LCURLY
        ( INTERPOLATION_MODIFIER? type MemberName=ID
            ( COLON semantic )? SEMI  )+
    RCURLY SEMI
    ;

constant_expression
    : (ID) => variable_expression
    | literal_value
    ;

literal_value
    : value= FLOAT
    | value= INT
    | value= ( TRUE_TOKEN | FALSE_TOKEN )
    ;

semantic
    : SEMANTIC
    | ID
    ;

SEMANTIC
    : 'POSITION' ('0'..'8')?
    | 'POSITIONT'
    | 'NORMAL' ('0'..'8')?
    | 'SV_POSITION'
    | 'COLOR' ('0'..'8')?
    | 'TEXCOORD' ('0'..'8')?
    | 'TESSFACTOR' ('0'..'8')?
    | 'PSIZE' ('0'..'8')?
    | 'DEPTH' ('0'..'8')?
    | 'VPOS'
    | 'VFACE'
    | 'FOG'
    | 'DIFFUSE'
    | 'TANGENT' ('0'..'8')?
    | 'BINORMAL' ('0'..'8')?
    | 'BLENDINDICES' ('0'..'8')?
    | 'BLENDWEIGHT' ('0'..'8')?
    ;

SEMI:               ';';
COMMA:              ',';
COLON:              ':';
EXTERN:             'extern';
NOINTERPOLATION:    'nointerpolation';
PRECISE:            'precise';
SHARED:             'shared';
GROUPSHARED:        'groupshared';
STATIC:             'static';
UNIFORM:            'uniform';
VOLATILE:           'volatile';
IN_TOKEN:           'in';
OUT_TOKEN:          'out';
INOUT:              'inout';
BREAK:              'break';
CONTINUE:           'continue';
RETURN:             'return';
DISCARD:            'discard';
DO:                 'do';
WHILE:              'while';
IF:                 'if';
ELSE:               'else';
FOR:                'for';
TECHNIQUE:          'technique';
PASS:               'pass';
VERTEX_SHADER:      'VertexShader';
PIXEL_SHADER:       'PixelShader';
COMPILE:            'compile';
LBRACKET:           '[';
RBRACKET:           ']';
LPAREN:             '(';
RPAREN:             ')';
LCURLY:             '{';
RCURLY:             '}';
DOT:                '.';
ASSIGN:             '=';
MUL_ASSIGN:         '*=';
DIV_ASSIGN:         '/=';
ADD_ASSIGN:         '+=';
SUB_ASSIGN:         '-=';
BITWISE_AND_ASSIGN: '&=';
BITWISE_OR_ASSIGN:  '|=';
BITWISE_XOR_ASSIGN: '^=';
BITWISE_SHIFTL_ASSIGN: '<<=';
BITWISE_SHIFTR_ASSIGN: '>>=';
QUESTION:           '?';
MUL:                '*';
DIV:                '/';
PLUSPLUS:           '++';
MINUSMINUS:         '--';
PLUS:               '+';
MINUS:              '-';
MOD:                '%';
EQUAL:              '==';
NOT_EQUAL:          '!=';
AND:                '&&';
OR:                 '||';
NOT:                '!';
BITWISE_NOT:        '~';
XOR:                '^^';
LT_TOKEN:           '<';
LTE:                '<=';
GT_TOKEN:                 '>';
GTE:                '>=';
BITWISE_AND:        '&';
BITWISE_OR:         '|';
BITWISE_XOR:        '^';
BITWISE_SHIFTL:     '<<';
BITWISE_SHIFTR:     '>>';
VOID_TOKEN:         'void';
TRUE_TOKEN:         'true';
FALSE_TOKEN:        'false';
STRUCT:             'struct';

TEXTURE:            T 'exture';
TEXTURE1D:          T 'exture1D';
TEXTURE1DARRAY:     T 'exture1DArray';
TEXTURE2D:          T 'exture2D';
TEXTURE2DARRAY:     T 'exture2DArray';
TEXTURE3D:          T 'exture3D';
TEXTURECUBE:        T 'extureCube';


SAMPLER_TYPE
    : 'sampler'
    | 'sampler1D'
    | 'sampler2D'
    | 'sampler3D'
    | 'samplerCUBE'
    | 'sampler_state'
    | 'SamplerState'
    ;

INTERPOLATION_MODIFIER
    : 'linear'
    | 'centroid'
    | 'nointerpolation'
    | 'noperspective'
    | 'sample'
    ;

MATRIX_TYPE
    : VECTOR_TYPE 'x' INDEX
    ;

VECTOR_TYPE
    : SCALAR_TYPE INDEX
    ;

SCALAR_TYPE
    : 'bool'
    | 'int'
    | 'float'
    | 'double'
    ;

STRING_TYPE
    : 'string'
    ;

ID  :	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
    ;

INT :	'0'..'9'+
    ;

FLOAT
    : FLOAT_NUMBER 'f'?
    ;

fragment
INDEX
    :  '1' | '2' | '3' | '4'
    ;

fragment
FLOAT_NUMBER
    :   ('-')?('0'..'9')+ '.' ('0'..'9')* EXPONENT?
    |   ('-')?'.' ('0'..'9')+ EXPONENT?
    |   ('-')?('0'..'9')+ EXPONENT
    ;

fragment
    T
    : 't' | 'T'
    ;

COMMENT
    :   '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

WS  :  (' '|'\r'|'\t'|'\u000C'|'\n') {$channel=HIDDEN;}
    ;

STRING
    :  '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
    ;

fragment
EXPONENT : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
ESC_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   OCTAL_ESC
    ;

fragment
OCTAL_ESC
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

