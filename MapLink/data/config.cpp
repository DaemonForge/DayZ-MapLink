class CfgPatches
{
	class Hive_Assest
	{
		units[]={""};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class cfgVehicles
{
	class Inventory_Base;
	class Paper;
	class HouseNoDestruct;
	class Hive_Terminal : HouseNoDestruct{
		scope = 1;
		displayName  = "Hive Terminal";
		model = "MapLink\data\hive_terminal.p3d";
	};
	class Ticket_base: Paper
	{
		scope=0;
	};
	class ND_Cher_Ticket: Ticket_base
	{
		scope=2;
		displayName="Chernarus Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		model="MapLink\data\ChernarusTicket.p3d";
		isMeleeWeapon=1;
		weight=2;
		absorbency=1;
		itemSize[]={1,2};
		rotationFlags=16;
		hitpoints=15;
	};
	class ND_Namalsk_Ticket: Ticket_base
	{
		scope=2;
		displayName="Namalsk Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		model="MapLink\data\NamalskTicket.p3d";
		isMeleeWeapon=1;
		weight=2;
		absorbency=1;
		itemSize[]={1,2};
		rotationFlags=16;
		hitpoints=15;
	};
};