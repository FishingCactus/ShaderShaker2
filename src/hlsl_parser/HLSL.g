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

    static AST::UnaryOperationExpression::Operation GetUnaryFromToken(
        const int type
        )
    {
        switch( type )
        {
            case PLUS: return AST::UnaryOperationExpression::Plus;
            case MINUS: return AST::UnaryOperationExpression::Minus;
            case NOT: return AST::UnaryOperationExpression::Not;
            case BITWISE_NOT: return AST::UnaryOperationExpression::BitwiseNot;
        }
        assert( !"error" );

        return AST::UnaryOperationExpression::Plus;
    }

    static AST::LiteralExpression::Type GetLiteralFromToken(
        const int type
        )
    {
        switch( type )
        {
            case INT: return AST::LiteralExpression::Int;
            case FLOAT: return AST::LiteralExpression::Float;
            case TRUE_TOKEN:
            case FALSE_TOKEN:
                return AST::LiteralExpression::Bool;
        }
        assert( !"error" );

        return AST::LiteralExpression::Int;
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
    | texture_declaration { declaration = $texture_declaration.declaration; }
    | sampler_declaration { declaration = $sampler_declaration.declaration; }
    | struct_definition { declaration = $struct_definition.definition; }
    | function_declaration { declaration = $function_declaration.declaration; }
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

statement returns [ AST::Statement * statement = 0 ]
    : ( lvalue_expression assignment_operator ) => assignment_statement
    | ( lvalue_expression self_modify_operator ) => post_modify_statement
    | variable_declaration
    | pre_modify_statement
    | expression_statement
    | block_statement {statement = $block_statement._statement;}
    | if_statement {statement = $if_statement._statement;}
    | iteration_statement {statement = $iteration_statement._statement;}
    | jump_statement {statement = $jump_statement.statement;}
    | SEMI {statement = new AST::EmptyStatement; }
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

block_statement returns [ AST::BlockStatement * _statement = 0 ]
    : { _statement = new AST::BlockStatement; } LCURLY (statement{_statement->AddStatement($statement.statement);})* RCURLY
    ;

expression_statement returns [ AST::ExpressionStatement * statement = 0 ]
    : expression SEMI { statement = new AST::ExpressionStatement( $expression.exp ); }
    ;

if_statement returns [ AST::IfStatement * _statement = 0 ] @init{ AST::Statement * else_statement = 0; }
    : IF LPAREN expression RPAREN a=statement
        // Design choice : Despite the risk of heavy recursion, I prefer to treat the
        // "else if " as a "else if_statement". This can be changed back if ever needed
        //( ELSE IF LPAREN expression  RPAREN statement )*
        ( ELSE b=statement {else_statement = $b.statement;})?
        { _statement = new AST::IfStatement( $expression.exp, $a.statement, else_statement ); }
    ;

iteration_statement returns [ AST::Statement * _statement = 0 ]
    : WHILE LPAREN expression RPAREN statement { _statement = new AST::WhileStatement( $expression.exp, $statement.statement ); }
    | DO statement WHILE LPAREN expression RPAREN SEMI  { _statement = new AST::DoWhileStatement( $expression.exp, $statement.statement ); }
    | FOR LPAREN ( assignment_statement | variable_declaration )
        equality_expression SEMI modify_expression RPAREN statement
    ;

modify_expression
    : (lvalue_expression assignment_operator ) =>
         lvalue_expression  assignment_operator expression
    | pre_modify_expression
    | post_modify_expression
    ;

jump_statement returns [ AST::Statement * statement = 0 ]
    : BREAK SEMI { statement = new AST::BreakStatement; }
    | CONTINUE SEMI { statement = new AST::ContinueStatement; }
    | RETURN { statement = new AST::ReturnStatement; }
        ( expression { static_cast<AST::ReturnStatement*>(statement)->m_Expression = std::shared_ptr<AST::Expression>( $expression.exp ); } )?
        SEMI
    | DISCARD SEMI { statement = new AST::DiscardStatement; }
    ;

lvalue_expression
    : variable_expression ( postfix_suffix )?
    ;

variable_expression returns [ AST::VariableExpression * exp = 0 ]
    : ID { exp = new AST::VariableExpression( $ID.text ); }
        ( LBRACKET expression RBRACKET { exp->m_SubscriptExpression = std::shared_ptr<AST::Expression>( $expression.exp ); } )?
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

cast_expression returns [ AST::Expression * exp = 0 ] @init { int array_size = -1; }
    : LPAREN type ( LBRACKET INT RBRACKET {array_size = atoi( $INT.text.c_str() );} )? RPAREN ce=cast_expression { exp = new AST::CastExpression( $type.type, array_size, $ce.exp ); }
    | unary_expression {exp = $unary_expression.exp; }
    ;

unary_expression returns [ AST::Expression * exp = 0 ]
    : op=(PLUS|MINUS|NOT|BITWISE_NOT) ue=unary_expression { exp = new AST::UnaryOperationExpression( GetUnaryFromToken( $op.type ), $ue.exp ); }
    | postfix_expression { exp = $postfix_expression.exp; }
    ;

postfix_expression returns [ AST::Expression * exp = 0 ]
    : primary_expression { exp = $primary_expression.exp; }( postfix_suffix { exp = new AST::PostfixExpression( exp, $postfix_suffix.suffix ); } )?
    ;

postfix_suffix returns [ AST::PostfixSuffix * suffix = 0 ]
    : DOT swizzle { suffix = new AST::Swizzle( $swizzle.text ); }
    | DOT primary_expression ( postfix_suffix )?
    // Not sure about primary_expression here, should be variable_expression or call, not more
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

primary_expression returns [ AST::Expression * exp = 0 ]
    : constructor { exp = $constructor.exp; }
    | call_expression { exp = $call_expression.exp; }
    | variable_expression { exp = $variable_expression.exp; }
    | literal_value { exp = $literal_value.exp; }
    | LPAREN expression RPAREN { exp = $expression.exp; }
    ;

constructor returns [ AST::ConstructorExpression * exp = 0 ]
    : type LPAREN argument_expression_list RPAREN { exp = new AST::ConstructorExpression( $type.type, $argument_expression_list.list ); }
    ;

call_expression returns [AST::CallExpression * exp = 0]
    : ID LPAREN argument_expression_list RPAREN { exp = new AST::CallExpression( $ID.text, $argument_expression_list.list ); }
    ;

argument_expression_list returns [ AST::ArgumentExpressionList * list = new AST::ArgumentExpressionList ]
    : ( a=expression { list->AddExpression( $a.exp ); }( COMMA b=expression { list->AddExpression( $b.exp ); } )*  )?
    ;

// Function

function_declaration returns [AST::FunctionDeclaration * declaration = 0 ]
    : { declaration = new AST::FunctionDeclaration; }( storage_class { declaration->AddStorageClass( $storage_class.storage ); } )* ( PRECISE )?
        ( type  { declaration->m_Type = std::shared_ptr<AST::Type>( $type.type ); } | VOID_TOKEN ) Name=ID { declaration->m_Name = $Name.text; }
        LPAREN ( argument_list { declaration->m_ArgumentList = std::shared_ptr<AST::ArgumentList>( $argument_list.list ); } )? RPAREN
        ( COLON semantic {declaration->m_Semantic = $semantic.text; })?
        LCURLY
            ( statement { declaration->AddStatement( $statement.statement ); } )*
        RCURLY
    ;

argument_list returns [ AST::ArgumentList * list = 0 ]
    : { list = new AST::ArgumentList; } a=argument { list->AddArgument( $a.argument ); } ( COMMA b=argument { list->AddArgument( $b.argument ); } )*
    ;

argument returns [ AST::Argument * argument = 0 ]
    : { argument = new AST::Argument; }
        ( input_modifier { argument->m_InputModifier = $input_modifier.text; } )?
        ( type_modifier { argument->m_TypeModifier = std::shared_ptr<AST::TypeModifier>( $type_modifier.modifier ); } )?
        type { argument->m_Type = std::shared_ptr<AST::Type>( $type.type ); }
        Name=ID { argument->m_Name = $Name.text; }
        ( COLON semantic { argument->m_Semantic = $semantic.text; } )?
        ( INTERPOLATION_MODIFIER {argument->m_InterpolationModifier = $INTERPOLATION_MODIFIER.text; } )?
        ( ASSIGN initial_value { argument->m_InitialValue = std::shared_ptr<AST::InitialValue>( $initial_value.value ); } )?
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

texture_declaration returns [ AST::TextureDeclaration * declaration = 0 ]
    : t=texture_type ID
    ( COLON semantic ) ?
    ( annotations ) ?
    { declaration = new AST::TextureDeclaration( $t.text, $ID.text, $semantic.text, $annotations.annotations ); }
    SEMI
    ;

sampler_declaration returns [AST::SamplerDeclaration * declaration = 0 ]
    : t=SAMPLER_TYPE
        Name=ID ( ASSIGN SAMPLER_TYPE )? { declaration = new AST::SamplerDeclaration( $t.text, $Name.text ); }
            LCURLY (sampler_body { declaration->AddBody( $sampler_body.body ); } )* RCURLY SEMI

    ;

sampler_body returns [ AST::SamplerBody * body = 0 ]
    : TEXTURE ASSIGN LT_TOKEN ID GT_TOKEN SEMI { body = new AST::SamplerBody( "texture", $ID.text ); }
    | Name=ID ASSIGN Value=ID SEMI { body = new AST::SamplerBody( $Name.text , $Value.text ); }
    ;

// Variables

variable_declaration returns [ AST::VariableDeclaration * declaration = 0 ]
    : { declaration = new AST::VariableDeclaration; }
        ( storage_class { declaration->AddStorageClass( $storage_class.storage ); } )*
        ( type_modifier { declaration->AddTypeModifier( $type_modifier.modifier ); })*
        type { declaration->SetType( $type.type ); }
        a=variable_declaration_body { declaration->AddBody( $a.body ); }
        ( COMMA b=variable_declaration_body { declaration->AddBody( $b.body ); } )* SEMI
    ;

variable_declaration_body returns [ AST::VariableDeclarationBody * body = 0 ]
    : { body = new AST::VariableDeclarationBody; }
        ID { body->m_Name = $ID.text; } ( LBRACKET INT RBRACKET { body->m_ArraySize = atoi( $INT.text.c_str() ); } )?
        ( COLON semantic {body->m_Semantic = $semantic.text; } ) ?
        ( COLON packoffset )?
        ( COLON register_rule ) ?
        ( annotations { body->m_Annotations = std::shared_ptr<AST::Annotations>( $annotations.annotations ); } ) ?
        ( ASSIGN initial_value { body->m_InitialValue = std::shared_ptr<AST::InitialValue>( $initial_value.value ); } ) ?
    ;

storage_class returns [ AST::StorageClass * storage = 0 ]
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

type_modifier returns [ AST::TypeModifier * modifier = 0 ]
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

annotations returns [ AST::Annotations * annotations = 0 ]
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

struct_definition returns [ AST::StructDefinition * definition = 0 ]
    : STRUCT Name=ID { definition = new AST::StructDefinition( $Name.text ); TypeTable.insert( $Name.text ); }
    LCURLY
        ( INTERPOLATION_MODIFIER? type MemberName=ID
            ( COLON semantic )? SEMI { definition->AddMember( $MemberName.text, $type.type, $semantic.text, $INTERPOLATION_MODIFIER ? $INTERPOLATION_MODIFIER.text : "" ); } )+
    RCURLY SEMI
    ;

constant_expression
    : (ID) => variable_expression
    | literal_value
    ;

literal_value  returns [ AST::Expression * exp = 0 ]
    : value =
        (
        FLOAT
        | INT
        | TRUE_TOKEN | FALSE_TOKEN
        ) { exp = new AST::LiteralExpression( GetLiteralFromToken( $value.type ), $value.text ); }
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

