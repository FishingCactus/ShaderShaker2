#ifndef ANNOTATION_NODE_H
    #define ANNOTATION_NODE_H


    namespace AST
    {
        struct AnnotationEntry;

        struct Annotations : Node
        {

            void AddEntry( AnnotationEntry * entry ) { m_AnnotationTable.emplace_back( entry ); }
            std::vector< std::shared_ptr< AnnotationEntry> >
                m_AnnotationTable;
        };

        struct AnnotationEntry : Node
        {
            AnnotationEntry( const std::string & type, const std::string & name, const std::string & value ) :
                m_Type( type ), m_Name( name ), m_Value( value ) {}

            std::string
                m_Type,
                m_Name,
                m_Value;
        };
    }

#endif