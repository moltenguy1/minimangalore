#ifndef PROPERTIES_PAUSERAINPUTPROPERTY_H
#define PROPERTIES_PAUSERAINPUTPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class Properties::InputProperty

    An input property adds the ability to handle user input to an entity.
    If an InputProperty is attached to an entity it can become the input
    focus entity. Global input focus is managed by the Game::FocusManager
    singleton.

    If you want the concept of an input focus in your application you should
    derive your own input property classes from the InputProperty class,
    because then the FocusManager will be aware of it (otherwise it will
    just ignore the entity).
    
    // this property will handle keybord during a pause session (state)
    // prety much the same as polarcamerainputproperty, even simpler...
    
    (C) je.a.le@wanadoo.Fr
*/
#include "game/property.h"
#include "attr/attributes.h"
#include "properties/inputproperty.h"
#include "managers/focusmanager.h"
#include "game/entity.h"
#include "input/server.h"

namespace Properties
{
class PauseInputProperty : public InputProperty
{
    DeclareRtti;
    DeclareFactory(PauseInputProperty);

public:
    /// constructor
    PauseInputProperty();
    /// destructor
    virtual ~PauseInputProperty();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    
    /// called when camera focus is obtained
    virtual void OnObtainFocus();			// grab the mouse control
    /// called when camera focus is lost
    virtual void OnLoseFocus();				// realse mouse control

	// this property will accept message from Input::Event::Id
	/// return true if message is accepted by a property
    virtual bool Accepts(Message::Msg* msg);
    /// handle a single message
    virtual void HandleMessage(Message::Msg* msg);

};

RegisterFactory(PauseInputProperty);

}; // namespace Property
//------------------------------------------------------------------------------
#endif
