#ifndef N_ROOTEXT_H
#define N_ROOTEXT_H
//----------------------------------------------------------------------------
/**
    @class RootExt
    @ingroup 
    @brief some extention functions for node
    
    (c) 2006    je.a.le@wanadoo.fr
*/
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

class RootExt
{
    public:
        /// constructor
		RootExt() {};
        /// destructor
		virtual ~RootExt() {};

		// return null if not found
		static nSceneNode *GetChildnRoot( nSceneNode *parent, nString child_name, bool no_recursive );
		static nSceneNode *GetChildnRootByPath( nSceneNode  *parent, nString path );
};

//----------------------------------------------------------------------------
/**
*/
nSceneNode *RootExt::GetChildnRoot( nSceneNode  *parent, nString child_name, bool no_recursive )
{
	// let's skip the assert would we....

	nSceneNode* curChild;
	
	// first look, check for direct child
    for (curChild = (nSceneNode*) parent->GetHead(); 
		curChild; 
		curChild = (nSceneNode*) curChild->GetSucc() )
    {
		//n_message( "%s\n", curChild->GetName() );
    
		if( curChild->GetName() == child_name.Get() ) {
			return( curChild );
		}    
	}			
				
	// if here, that wasn't really successfull, let's try recurs
	if( no_recursive == true ) {
		return( 0 );
	}
	
	// the use of 2 loops allow the scan of level 1 before others., 
   for (curChild = (nSceneNode*) parent->GetHead(); 
		curChild; 
		curChild = (nSceneNode*) curChild->GetSucc() )
    {
		// already no it's not on "level 1"
    	nSceneNode* curChild2 = GetChildnRoot( (nSceneNode*) curChild->GetHead(), child_name, no_recursive );
    
		if( curChild2 != 0 ) {
			return( curChild2 );
		}
	}			
	
	// well, seem it isn't the day...
	return( 0 );	
}

//----------------------------------------------------------------------------
// same but use a path to the node (ex : opelblitz/model/static_0_0 )
nSceneNode *RootExt::GetChildnRootByPath(nSceneNode  *parent, nString path )
{
	nSceneNode* curChild;
	
	nArray<nString> nameTokens;
    int numTokens = path.Tokenize("/", nameTokens);
	
	if( numTokens <= 0 ) {
		return( 0 );
	}
		
	// check for nameTokens[0]
    for (curChild = (nSceneNode*) parent->GetHead(); 
		curChild; 
		curChild = (nSceneNode*) curChild->GetSucc() )
    {
		//n_message( "%s\n", curChild->GetName() );
		//nString	a(curChild->GetName());
		//nString b(nameTokens[0].Get());
		
		if( curChild->GetName() == nString( nameTokens[0].Get() ) ) {
			if( numTokens > 1 ) {
				// Fix me : think it's faster than concat array element
				return( GetChildnRootByPath( curChild, 
					path.ExtractRange( nameTokens[0].Length() + 1, 
					path.Length() - 1 - nameTokens[0].Length() ) ) );
			}
			
			// last : return;
			return( curChild );
		}    
	}			

	// not found
	return( 0 );
}

//----------------------------------------------------------------------------
#endif 

