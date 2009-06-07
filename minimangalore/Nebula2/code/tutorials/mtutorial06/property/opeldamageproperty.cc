//------------------------------------------------------------------------------
//  properties/Polarcameraproperty.cc
//  (C) je.a.le@wanadoo.fr
//------------------------------------------------------------------------------
#include "mtutorial06/property/opeldamageproperty.h"

#include "managers/focusmanager.h"
#include "managers/entitymanager.h"
#include "graphics/server.h"
#include "graphics/cameraentity.h"
#include "graphics/entity.h"
#include "game/entity.h"

// handler some math functions nmath.h don't....
#include "mtutorial06/mathext.h"
#include "mtutorial06/rootext.h"

// the message class used for communication
#include "mtutorial06/message/damagevalues.h"
#include "properties/graphicsproperty.h"
#include "scene/nmaterialnode.h"

// Polar camera specific attributes
namespace Attr
{
	DefineBool(OpelAsDamage);
};

namespace Properties
{
ImplementRtti(Properties::OpelDamageProperty, Game::Property);
ImplementFactory(Properties::OpelDamageProperty);

using namespace Game;
using namespace Managers;
using namespace Message;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
OpelDamageProperty::OpelDamageProperty() 
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
OpelDamageProperty::~OpelDamageProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This adds the default attributes to the property. (and set the Attr... )
*/
void OpelDamageProperty::SetupDefaultAttributes()
{
    Game::Property::SetupDefaultAttributes();

	GetEntity()->SetBool( Attr::OpelAsDamage, false );
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a message handler you must implement the Accepts() method to return
    true for each message that is accepted. By default no messages are accepted.
*/
bool OpelDamageProperty::Accepts(Message::Msg* msg)
{
	n_assert(msg);
	
	// accept input message
	if( msg->CheckId(Message::DamageValues::Id) ) {
		return true;
	}

	// let's see if message inherit
    return( Message::Port::Accepts(msg) );
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a message handler you must implement the HandleMessage() method to
    process the incoming messages. Please note that HandleMessage() will
    not be called "automagically" because the Property doesn't know at which
    point in the frame you want to handle pending messages.

    Thus, you must call the HandlePendingMessages() yourself from either OnBeginFrame(),
    OnMoveBefore(), OnMoveAfter() or OnRender().

    The simple rule is: if you override the Accepts() method, you must also call
    HandlePendingMessages() either in OnBeginFrame(), OnMoveBefore(), OnMoveAfter()
    or OnRender()
*/
void OpelDamageProperty::HandleMessage(Message::Msg* msg)
{
    n_assert(msg);
   
	// ok, handled but which type ( here won't care about inherit message )
	if( msg->CheckId( Message::DamageValues::Id ) ) {
		DamageValues *	val = static_cast<DamageValues *>(msg);
		
		// Why static_0_0 ??? because we set it while modelling or 
		// simply look at opelblirz.n2 !!!! remember, on this tutorial
		// only take care about one mesh : the chassis
		if( val->GetEntityName() == "opelblitz/model/static_0_0" ) {
			// Here the IMPORTANT POINT of the tutorial, how to retrieve
			// ref to the node static_0_0
			
			// here it would be :
			//	Ptr<nRoot> node = topnode->GetHead()->GetHead()->GetHead();
			// but I'm going to use a path instead
				
			Ptr<Properties::GraphicsProperty> gp = (Properties::GraphicsProperty *)(GetEntity()->FindProperty( GraphicsProperty::RTTI ));
			Ptr<Graphics::Entity> e = gp->GetGraphicsEntities()[0];
			
			// get the child object and cast as materialnode (because we know it opelblitz.n2)
			Ptr<nMaterialNode> mn = (nMaterialNode *)(
				RootExt::GetChildnRootByPath( e->GetResource().GetNode(), val->GetEntityName() ) );
			
			// now change the texture according to damage level
			switch( val->GetType() ) {
				case DamageValues::Type::D_NONE :
					mn->SetTexture( nShaderState::DiffMap0, "textures:examples/blitz_fahrgestell.dds" );
					break;
				case DamageValues::Type::D_LEVEL_1 :
					mn->SetTexture( nShaderState::DiffMap0, "textures:examples/blitz_fahrgestell_dm1.dds" );
					break;
				case DamageValues::Type::D_LEVEL_2 :
					mn->SetTexture( nShaderState::DiffMap0, "textures:examples/blitz_fahrgestell_dm2.dds" );
					break;
				case DamageValues::Type::D_LEVEL_YELLOW :
					mn->SetTexture( nShaderState::DiffMap0, "textures:examples/yellow.dds" );
					break;
				case DamageValues::Type::D_LEVEL_RED :
					mn->SetTexture( nShaderState::DiffMap0, "textures:examples/red.dds" );
					break;
				default :
					break;
			}
		}
	
		return;
	}

	// not here !
    Message::Port::HandleMessage(msg);
}

}; // namespace Properties
