#include "properties/inputproperty.h"

namespace Properties
{

class GameMenuInputProperty : 
	public InputProperty
{
public:
	DeclareRtti;
	DeclareFactory(GameMenuInputProperty);

public:
	GameMenuInputProperty();
	virtual ~GameMenuInputProperty();

	virtual void SetupDefaultAttributes();

	virtual void OnObtainFocus();


};

RegisterFactory(GameMenuInputProperty);

};