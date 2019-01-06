class PlayerSystem {
	int playerHP;
	int* EManager;

	PlayerSystem(int* Emanager)
	{
		playerHP = 123;
		EManager = Emanager;
		void*(*f)(void*) = AttackPlayer;
		//Callback* cb = new Callback(&AttackPlayer);
		//Emanager->AddEvent(1,"Attack",)
	}

	~PlayerSystem()
	{

	}

	void* AttackPlayer(void* dmg)
	{
		playerHP -= *((int*)dmg);
	}
};