#include "hlsl_printer.h"

#include <cassert>
#include "utils/indentation.h"
#include "ast/node.h"

namespace AST
{

    void HLSLPrinter::Visit( Node & node )
    {
        assert( !"Unsupported node type, implement in base class" );
    }

    void HLSLPrinter::Visit( TranslationUnit & translation_unit )
    {
        {
            std::vector< Base::ObjectRef<GlobalDeclaration> >::iterator it, end;
            it = translation_unit.m_GlobalDeclarationTable.begin();
            end = translation_unit.m_GlobalDeclarationTable.end();

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        {
            std::vector< Base::ObjectRef<Technique> >::iterator it, end;
            it = translation_unit.m_TechniqueTable.begin();
            end = translation_unit.m_TechniqueTable.end();

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }
        }
    }

    void HLSLPrinter::Visit( VariableDeclaration & variable_declaration )
    {


    }

    void HLSLPrinter::Visit( IntrinsicType & type )
    {
        m_Stream << type.m_Name;
    }

    void HLSLPrinter::Visit( UserDefinedType & type )
    {
        m_Stream << type.m_Name;
    }

    void HLSLPrinter::Visit( SamplerType & type )
    {
        m_Stream << type.m_Name;
    }

    void HLSLPrinter::Visit( TypeModifier & modifier )
    {


    }

    void HLSLPrinter::Visit( StorageClass & storage_class )
    {


    }

    void HLSLPrinter::Visit( VariableDeclarationBody & body )
    {
        m_Stream << body.m_Name;

        if( body.m_ArraySize )
        {
            m_Stream << "[" << body.m_ArraySize << "]";
        }

        if( !body.m_Semantic.empty() )
        {
            m_Stream << " : " << body.m_Semantic;
        }

        if( body.m_Annotations )
        {
            body.m_Annotations->Visit( *this );
        }

        if( body.m_InitialValue )
        {
            m_Stream << " = ";

            body.m_InitialValue->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( TextureDeclaration & declaration )
    {


    }

    void HLSLPrinter::Visit( SamplerDeclaration & declaration )
    {


    }

    void HLSLPrinter::Visit( SamplerBody & body )
    {


    }

    void HLSLPrinter::Visit( StructDefinition & definition )
    {


    }

    void HLSLPrinter::Visit( FunctionDeclaration & declaration )
    {
        {
            std::vector< Base::ObjectRef<StorageClass> >::iterator it, end;
            it = declaration.m_StorageClassTable.begin();
            end = declaration.m_StorageClassTable.end();

            for(; it!=end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        if( declaration.m_Type )
        {
            declaration.m_Type->Visit( *this );
        }
        else
        {
            m_Stream << "void";
        }
        m_Stream << " " << declaration.m_Name << "(";
        if( declaration.m_ArgumentList )
        {
            declaration.m_ArgumentList->Visit( * this );
        }
        m_Stream << ")";

        if( !declaration.m_Semantic.empty() )
        {
            m_Stream << " : " << declaration.m_Semantic;
        }

        m_Stream << endl_ind << "{" << inc_ind << endl_ind;

        {
            std::vector< Base::ObjectRef<Statement> >::iterator it, end;
            it = declaration.m_StatementTable.begin();
            end = declaration.m_StatementTable.end();

            for(; it!=end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        m_Stream << dec_ind << endl_ind << "}" << endl_ind;
    }

    void HLSLPrinter::Visit( ArgumentList & list )
    {
        bool first = true;

        {
            std::vector< Base::ObjectRef<Argument> >::iterator it, end;
            it = list.m_ArgumentTable.begin();
            end = list.m_ArgumentTable.end();

            for(; it!=end; ++it )
            {
                if( !first ) m_Stream << ", ";
                first = false;
                (*it)->Visit( *this );
            }
        }
    }

    void HLSLPrinter::Visit( Argument & argument )
    {
        if( !argument.m_InputModifier.empty() )
        {
            m_Stream << argument.m_InputModifier << " ";
        }

        if( argument.m_TypeModifier )
        {
            argument.m_TypeModifier->Visit( *this );
        }

        m_Stream << argument.m_Type->m_Name << " " << argument.m_Name;

        if( !argument.m_Semantic.empty() )
        {
            m_Stream << " : " << argument.m_Semantic;
        }

        if( !argument.m_InterpolationModifier.empty() )
        {
            m_Stream << " " << argument.m_InterpolationModifier;
        }

        if( argument.m_InitialValue )
        {
            m_Stream << " = ";

            argument.m_InitialValue->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( LiteralExpression & expression )
    {

        m_Stream << expression.m_Value;
    }

    void HLSLPrinter::Visit( VariableExpression & expression )
    {
        m_Stream << expression.m_Name;

        if( expression.m_SubscriptExpression )
        {
            m_Stream << '[';
            expression.m_SubscriptExpression->Visit( *this );
            m_Stream << ']';
        }
    }

    void HLSLPrinter::Visit( UnaryOperationExpression & expression )
    {
        m_Stream << expression.m_Operation << "( ";
        expression.m_Expression->Visit( *this );
        m_Stream << " )";
    }

    void HLSLPrinter::Visit( BinaryOperationExpression & expression )
    {
        m_Stream << "( ";
        expression.m_LeftExpression->Visit( *this );
        m_Stream << " ) " << expression.m_Operation << " ( ";
        expression.m_RightExpression->Visit( *this );
        m_Stream << " )";
    }

    void HLSLPrinter::Visit( CallExpression & expression )
    {
        m_Stream << expression.m_Name << "(";

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        m_Stream << ")";
    }

    void HLSLPrinter::Visit( ArgumentExpressionList & list )
    {
        size_t
            index,
            count;

        count = list.m_ExpressionList.size();

        for( index = 0; index < count; ++index )
        {
            if( index != 0 ) m_Stream << ", ";
            list.m_ExpressionList[ index ]->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( Swizzle & swizzle )
    {
        m_Stream << "." << swizzle.m_Swizzle;
    }

    void HLSLPrinter::Visit( PostfixSuffixCall & postfix_suffix )
    {
        m_Stream << ".";
        postfix_suffix.m_CallExpression->Visit( *this );

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( PostfixSuffixVariable & postfix_suffix )
    {
        m_Stream << ".";
        postfix_suffix.m_VariableExpression->Visit( *this );

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( ConstructorExpression & expression )
    {
        expression.m_Type->Visit( *this );
        m_Stream << "(";

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        m_Stream << ")";
    }

    void HLSLPrinter::Visit( ConditionalExpression & expression )
    {
        //:TODO: generate parenthesis only when required ( using operator precedence )
        m_Stream << "( ";
        expression.m_Condition->Visit( *this );
        m_Stream << " ) ? ( ";
        expression.m_IfTrue->Visit( *this );
        m_Stream << " ) : ( ";
        expression.m_IfFalse->Visit( *this );
        m_Stream << " )";
    }

    void HLSLPrinter::Visit( LValueExpression & expression )
    {
        expression.m_VariableExpression->Visit( *this );

        if( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( PreModifyExpression & expression )
    {
        m_Stream << expression.m_Operator;
        expression.m_Expression->Visit( *this );
    }

    void HLSLPrinter::Visit( PostModifyExpression & expression )
    {
        expression.m_Expression->Visit( *this );
        m_Stream << expression.m_Operator;
    }

    void HLSLPrinter::Visit( CastExpression & expression )
    {
        m_Stream << "( ";
        expression.m_Type->Visit( *this );

        if( expression.m_ArraySize != -1 )
        {
            m_Stream << "[" << expression.m_ArraySize << "]";
        }

        m_Stream <<" )( ";

        expression.m_Expression->Visit( *this );

        m_Stream << " )";
    }

    void HLSLPrinter::Visit( AssignmentExpression & expression )
    {
        expression.m_LValueExpression->Visit( *this );
        m_Stream << " " << expression.m_Operator << " ";
        expression.m_Expression->Visit( *this );
    }

    void HLSLPrinter::Visit( PostfixExpression & expression )
    {
        expression.m_Expression->Visit( *this );

        if( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( ReturnStatement & statement )
    {
        m_Stream << "return";

        if( statement.m_Expression )
        {
            m_Stream << " ";
            statement.m_Expression->Visit( *this );
        }

        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( BreakStatement & statement )
    {
        m_Stream << "break;" << endl_ind;
    }

    void HLSLPrinter::Visit( ContinueStatement & statement )
    {
        m_Stream << "continue;" << endl_ind;
    }

    void HLSLPrinter::Visit( DiscardStatement & statement )
    {
        m_Stream << "discard;" << endl_ind;
    }

    void HLSLPrinter::Visit( EmptyStatement & statement )
    {
        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( ExpressionStatement & statement )
    {
        statement.m_Expression->Visit( *this );
        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( IfStatement & statement )
    {
        m_Stream << "if( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " ) ";
        statement.m_ThenStatement->Visit( *this );

        if( statement.m_ElseStatement)
        {
            m_Stream << "else ";
            statement.m_ElseStatement->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( WhileStatement & statement )
    {
        m_Stream << "while( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " ) ";
        statement.m_Statement->Visit( *this );
    }

    void HLSLPrinter::Visit( DoWhileStatement & statement )
    {
        m_Stream << "do ";
        statement.m_Statement->Visit( *this );
        m_Stream << "while( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " );\n";
    }

    void HLSLPrinter::Visit( BlockStatement & statement )
    {
        if( statement.m_StatementTable.empty() )
        {
            m_Stream << "{}" << endl_ind;
        }
        else
        {
            m_Stream << "{" << inc_ind << endl_ind;

            std::vector< Base::ObjectRef<Statement> >::iterator
                it, end = statement.m_StatementTable.end();

            for ( it = statement.m_StatementTable.begin(); it != end; ++it )
            {
                (*it)->Visit( *this );
            }

            m_Stream << dec_ind << endl_ind << "}" << endl_ind;
        }
    }

    void HLSLPrinter::Visit( AssignmentStatement & statement )
    {
        statement.m_Expression->Visit( *this );
        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( VariableDeclarationStatement & statement )
    {
        {
            std::vector< Base::ObjectRef<StorageClass> >::iterator it, end;
            it = statement.m_StorageClass.begin();
            end = statement.m_StorageClass.end();
            bool first = true;
            for( ;it != end; ++it )
            {
                if(!first) m_Stream << "," << endl_ind;
                first = false;
                (*it)->Visit( *this );
            }
        }

        {
            std::vector< Base::ObjectRef<TypeModifier> >::iterator it, end;
            it = statement.m_TypeModifier.begin();
            end = statement.m_TypeModifier.end();

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        m_Stream << statement.m_Type->m_Name;

        {
            std::vector< Base::ObjectRef<VariableDeclarationBody> >::iterator it, end;
            it = statement.m_BodyTable.begin();
            end = statement.m_BodyTable.end();

            m_Stream << inc_ind << endl_ind;

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }

            m_Stream << dec_ind;
        }

        m_Stream << ";" << endl_ind;
    }

}
