#ifndef OBJECT_REF_H
    #define OBJECT_REF_H

    #include <cassert>

    namespace Base
    {
        template<typename ObjectType>
        class ObjectRef
        {
        public:

            ObjectRef() : m_Object( 0 ) {}
            ObjectRef( ObjectType * object ) : m_Object( 0 ) { Set( object );}
            ObjectRef( const ObjectRef & object ) : m_Object( 0 ) { Set( object.m_Object );}
            ObjectRef( ObjectRef && object ) : m_Object( object.m_Object ) { object.m_Object = 0; }
            ~ObjectRef(){ Set( 0 ); }

            ObjectRef<ObjectType> & operator=( const ObjectRef & object )
            {
                Set( object.m_Object );
                return *this;
            }

            ObjectRef<ObjectType> & operator=( ObjectRef && object )
            {
                Set( 0 );
                m_Object = object.m_Object;
                object.m_Object = 0;
                return *this;
            }

            operator bool() const { return !!m_Object; }

            bool operator <( const ObjectRef & other ) const { return m_Object < other.m_Object; }

            ObjectType * operator->() { assert( m_Object ); return m_Object; }
            ObjectType & operator*() { assert( m_Object ); return *m_Object; }
            const ObjectType * operator->() const { assert( m_Object ); return m_Object; }
            const ObjectType & operator*() const { assert( m_Object ); return *m_Object; }


        private:

            void Set( ObjectType * object )
            {
                if( m_Object )
                {
                    m_Object->RemoveRef();
                }

                m_Object = object;

                if( m_Object )
                {
                    m_Object->AddRef();
                }
            }

            ObjectType
                * m_Object;
        };
    }


#endif