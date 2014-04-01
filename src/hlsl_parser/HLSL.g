//

grammar HLSL;

options {
    backtrack = true;
    language = Cpp;
}

@lexer::traits
{
    #include <sstream>
    #include "hlsl_traits.h"
    using std::string;
}

@parser::includes
{
    #include "HLSLLexer.hpp"
    #include <iostream>
    #include <string>
    #include <set>
    #include <algorithm>
    #include "ast/node.h"
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

    static AST::BinaryOperationExpression::Operation GetFromToken(
        const int type
        )
    {
        switch( type )
        {
            case EQUAL: return AST::BinaryOperationExpression::Equality;
            case NOT_EQUAL: return AST::BinaryOperationExpression::Difference;
            case LT_TOKEN: return AST::BinaryOperationExpression::LessThan;
            case GT_TOKEN: return AST::BinaryOperationExpression::GreaterThan;
            case LTE: return AST::BinaryOperationExpression::LessThanOrEqual;
            case GTE: return AST::BinaryOperationExpression::GreaterThanOrEqual;
            case BITWISE_SHIFTL: return AST::BinaryOperationExpression::BitwiseLeftShift;
            case BITWISE_SHIFTR: return AST::BinaryOperationExpression::BitwiseRightShift;
            case PLUS: return AST::BinaryOperationExpression::Addition;
            case MINUS: return AST::BinaryOperationExpression::Subtraction;
            case MUL: return AST::BinaryOperationExpression::Multiplication;
            case DIV: return AST::BinaryOperationExpression::Division;
            case MOD: return AST::BinaryOperationExpression::Modulo;
        }

        assert( !"error" );
        return AST::BinaryOperationExpression::LogicalOr;
    }

    std::set<std::string>
        TypeTable;
}

translation_unit returns [ AST::TranslationUnit * unit ]
    : { unit = new AST::TranslationUnit;}
        ( global_declaration{ unit->AddGlobalDeclaration( $global_declaration.declaration ); })* technique* EOF
    ;

global_declaration returns [ AST::GlobalDeclaration * declaration = 0 ]
    : variable_declaration { declaration = $variable_declaration.declaration; }
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

expression returns [ AST::Expression * exp = 0 ]
    : conditional_expression { exp = $conditional_expression.exp; }
    ;

conditional_expression returns [ AST::Expression * exp = 0 ]
    : logical_or_expression
        ( QUESTION a=expression COLON b=conditional_expression
            {
                AST::ConditionalExpression * ce = new AST::ConditionalExpression;
                ce->m_Condition = std::shared_ptr<AST::Expression>( $logical_or_expression.exp );
                ce->m_IfTrue = std::shared_ptr<AST::Expression>( $a.exp );
                ce->m_IfFalse = std::shared_ptr<AST::Expression>( $b.exp );
                exp = ce;
             }
            )?
        { if( exp == 0 ) { exp = $logical_or_expression.exp; } }
    ;

logical_or_expression returns [ AST::Expression * exp = 0 ]
    :  a=logical_and_expression {exp = $a.exp;} ( OR b=logical_and_expression { exp = new AST::BinaryOperationExpression( AST::BinaryOperationExpression::LogicalOr, exp, $b.exp ); }  )*
    ;

logical_and_expression returns [ AST::Expression * exp = 0 ]
    : a=inclusive_or_expression {exp = $a.exp;} ( AND b=inclusive_or_expression { exp = new AST::BinaryOperationExpression( AST::BinaryOperationExpression::LogicalAnd, exp, $b.exp ); } )*
    ;

inclusive_or_expression returns [ AST::Expression * exp = 0 ]
    : a=exclusive_or_expression {exp = $a.exp;}( BITWISE_OR b=exclusive_or_expression { exp = new AST::BinaryOperationExpression( AST::BinaryOperationExpression::BitwiseOr, exp, $b.exp ); } )*
    ;

exclusive_or_expression returns [ AST::Expression * exp = 0 ]
  : a=and_expression {exp = $a.exp;}( BITWISE_XOR b=and_expression { exp = new AST::BinaryOperationExpression( AST::BinaryOperationExpression::BitwiseXor, exp, $b.exp ); } )*
  ;

and_expression returns [ AST::Expression * exp = 0 ]
    : a=equality_expression {exp = $a.exp;} ( BITWISE_AND b=equality_expression { exp = new AST::BinaryOperationExpression( AST::BinaryOperationExpression::BitwiseAnd, exp, $b.exp ); } )*
    ;

equality_expression returns [ AST::Expression * exp = 0 ]
    : a=relational_expression {exp = $a.exp;} (op=(EQUAL|NOT_EQUAL) b=relational_expression { exp = new AST::BinaryOperationExpression( GetFromToken( $op.type ), exp, $b.exp ); } )*
    ;

relational_expression returns [ AST::Expression * exp = 0 ]
    : a=shift_expression {exp = $a.exp;} ( op=(LT_TOKEN|GT_TOKEN|LTE|GTE) b=shift_expression { exp = new AST::BinaryOperationExpression( GetFromToken( $op.type ), exp, $b.exp ); } )?
    ;

shift_expression returns [ AST::Expression * exp = 0 ]
    : a=additive_expression {exp = $a.exp;} (op=(BITWISE_SHIFTL|BITWISE_SHIFTR) b=additive_expression { exp = new AST::BinaryOperationExpression( GetFromToken( $op.type ), exp, $b.exp ); } )*
    ;

additive_expression returns [ AST::Expression * exp = 0 ]
    : a=multiplicative_expression {exp = $a.exp;} ( op=(PLUS|MINUS) b=multiplicative_expression { exp = new AST::BinaryOperationExpression( GetFromToken( $op.type ), exp, $b.exp ); } )*
    ;

multiplicative_expression returns [ AST::Expression * exp = 0 ]
    : a=cast_expression {exp = $a.exp;} ( op=(MUL|DIV|MOD) b=cast_expression { exp = new AST::BinaryOperationExpression( GetFromToken( $op.type ), exp, $b.exp ); } )*
    ;

cast_expression returns [ AST::Expression * exp = 0 ]
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

variable_declaration returns [ AST::VariableDeclaration * declaration ]
    : { declaration = new AST::VariableDeclaration; }
        ( storage_class { declaration->AddStorageClass( $storage_class.storage ); } )*
        ( type_modifier { declaration->AddTypeModifier( $type_modifier.modifier ); })*
        type { declaration->SetType( $type.type ); }
        a=variable_declaration_body { declaration->AddBody( $a.body ); }
        ( COMMA b=variable_declaration_body { declaration->AddBody( $b.body ); } )* SEMI
    ;

variable_declaration_body returns [ AST::VariableDeclarationBody * body ]
    : { body = new AST::VariableDeclarationBody; }
        ID { body->m_Name = $ID.text; } ( LBRACKET INT RBRACKET { body->m_ArraySize = atoi( $INT.text.c_str() ); } )?
        ( COLON semantic {body->m_Semantic = $semantic.text; } ) ?
        ( COLON packoffset )?
        ( COLON register_rule ) ?
        ( annotations ) ?
        ( ASSIGN initial_value { body->m_InitialValue = std::shared_ptr<AST::InitialValue>( $initial_value.value ); } ) ?
    ;

storage_class returns [ AST::StorageClass * storage ]
    :
    s =
    ( EXTERN
    | NOINTERPOLATION
    | PRECISE
    | SHARED
    | GROUPSHARED
    | STATIC
    | UNIFORM
    | VOLATILE
    ) { storage = new AST::StorageClass( $s.text ); }
    ;

type_modifier returns [ AST::TypeModifier * modifier ]
    : m =
    ('const'
    | 'row_major'
    | 'column_major'
    ) { modifier = new AST::TypeModifier( $m.text ); }
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

initial_value returns [ AST::InitialValue * value = 0 ]
    :
    { value = new AST::InitialValue; }
    expression { value->AddExpression( $expression.exp ); }
    | { value = new AST::InitialValue; } { value->m_Vector = true; } LCURLY a=expression { value->AddExpression( $a.exp ); }
        ( COMMA b=expression { value->AddExpression( $b.exp ); } )* RCURLY
    ;

type returns [ AST::Type * type = 0 ]
    : intrinsic_type { type = $intrinsic_type.type; }
        | user_defined_type { type = $user_defined_type.type; }
        | SAMPLER_TYPE { type = new AST::SamplerType( $SAMPLER_TYPE.text ); }
    ;

intrinsic_type returns [ AST::IntrinsicType * type = 0 ]
    : t =
        ( MATRIX_TYPE
        | VECTOR_TYPE
        | SCALAR_TYPE
        ) { type = new AST::IntrinsicType( $t.text ); }
    ;

user_defined_type returns [ AST::UserDefinedType * type = 0 ]
    : ID  { TypeTable.find( $ID.text) != TypeTable.end() }? => { type = new AST::UserDefinedType( $ID.text ); }
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

ID  :   ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
    ;

INT :   '0'..'9'+
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

