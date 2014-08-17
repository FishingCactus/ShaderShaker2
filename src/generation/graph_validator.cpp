#include "graph_validator.h"

#include "function_definition.h"
#include "graph_node.h"
#include <sstream>

namespace Generation
{

    GraphValidator::GraphValidator(
        Base::ErrorHandlerInterface & error_handler
        ) :
        m_ErrorHandler( &error_handler ),
        m_GraphHasErrors( false )
    {

    }

    void GraphValidator::Visit( const GraphNode & node )
    {
        if( m_VisitedNodeSet.find( &node ) != m_VisitedNodeSet.end() )
        {
            return;
        }

        m_VisitedNodeSet.insert( &node );

        if( !node.HasFunctionDefinition() )
        {
            return;
        }

        const FunctionDefinition
            & definition = node.GetFunctionDefinition();

        std::map<std::string, std::string>::const_iterator it, end;

        it = definition.GetSemanticTypeTable().begin();
        end = definition.GetSemanticTypeTable().end();

        for( ; it != end; ++it )
        {
            std::map<std::string, std::string>::iterator value;

            value = m_SemanticToTypeMap.find( (*it).first );

            if( value == m_SemanticToTypeMap.end() )
            {
                m_SemanticToTypeMap.insert( *it );
            }
            else
            {
                if( (*value).second != (*it).second )
                {
                    std::ostringstream
                        message;

                    message << "Incompatible type for semantic " << (*it).first
                        << " in function " << definition.GetName() << ": previously seen type was "
                        << (*value).second << " but defined here as " << (*it).second;

                    m_ErrorHandler->ReportError( message.str(), "" );

                    m_GraphHasErrors = true;
                }
            }
        }
    }
}