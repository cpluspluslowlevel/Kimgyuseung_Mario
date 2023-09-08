#ifndef _LIBRARY_GLOBAL_H_
#define _LIBRARY_GLOBAL_H_

#define LIBRARY_NAMESPACE_NAME		Library
#define LIBRARY_NAMESPACE_FULL_NAME	LIBRARY_NAMESPACE_NAME
#define LIBRARY_NAMESPACE_BEGIN		namespace LIBRARY_NAMESPACE_NAME{
#define LIBRARY_NAMESPACE_END		}
#define LIBRARY_NAMESPACE_USING		using namespace LIBRARY_NAMESPACE_FULL_NAME;

LIBRARY_NAMESPACE_BEGIN

template< typename T >
void safe_delete( T& object ){
	delete object;
	object = nullptr;
}

template< typename T >
class CDestructor_Technique{
public:
	CDestructor_Technique( T functor ) : m_functor{ functor }{}
	CDestructor_Technique( const CDestructor_Technique&  ) = default;
	CDestructor_Technique( CDestructor_Technique&&  ) = default;
	~CDestructor_Technique(){ m_functor(); }

	
private:
	T m_functor;
};

template< typename Functor_Type >
CDestructor_Technique< Functor_Type > make_destructor_technique( Functor_Type functor ){
	return CDestructor_Technique< Functor_Type >{ functor };
}

template< typename Type >
class CSingleton{
public:

	template< typename ...Args >
	static void create( Args ...args ){
		Type** pptr = get_pptr();
		if( *pptr == nullptr ){
			*pptr = new Type{ args... };
		}
	}

	static Type* get(){
		return *get_pptr();
	}

	static void release(){
		delete *get_pptr();
	}

private:
	static Type** get_pptr(){
		static Type* ptr = nullptr;
		return &ptr;
	}
};

#define SINGLETON_DEFAULT( type )	private:                         \
									type()              = default;   \
									type( const type& ) = delete;	 \
									type( type&& )      = delete;    \
									~type()             = default;   \
									friend Library::CSingleton< type >;

#define SINGLETON( type, ... ) private:                         \
							   type( __VA_ARGS__ );				\
							   type( const type& ) = delete;	\
							   type( type&& )      = delete;    \
							   ~type()             = default;   \
							   friend Library::CSingleton< type >;


LIBRARY_NAMESPACE_END

#endif