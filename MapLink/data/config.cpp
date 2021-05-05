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
		scope=2;
		displayName="Chernarus Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		model="MapLink\data\Ticket_Base.p3d";
		isMeleeWeapon=1;
		weight=2;
		absorbency=1;
		itemSize[]={1,2};
		rotationFlags=16;
		hitpoints=15;
		hiddenSelections[]=
		{
			"Ticket_ColorBase"
		};
		hiddenSelectionsTextures[]=
		{
			"MapLink\data\textures\tickets\Blank Ticket_Color.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MapLink\data\textures\tickets\Ticket.rvmat"
		};
	};
	class Chernarus_Ticket: Ticket_base
	{
		scope=2;
		displayName="Chernarus Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		hiddenSelectionsTextures[]=
		{
			"MapLink\data\textures\tickets\Chernarus Tickets_Color.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MapLink\data\textures\tickets\Ticket.rvmat"
		};
	};
	class Livonia_Ticket: Ticket_base
	{
		scope=2;
		displayName="Livonia Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		hiddenSelectionsTextures[]=
		{
			"MapLink\data\textures\tickets\Livonia Ticket_Color.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MapLink\data\textures\tickets\Ticket.rvmat"
		};
	};
	class Namalsk_Ticket: Ticket_base
	{
		scope=2;
		displayName="Namalsk Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		hiddenSelectionsTextures[]=
		{
			"MapLink\data\textures\tickets\Namalsk Tickets_Color.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MapLink\data\textures\tickets\Ticket.rvmat"
		};
	};
	class Chiemsee_Ticket: Ticket_base
	{
		scope=2;
		displayName="Chiemsee Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		hiddenSelectionsTextures[]=
		{
			"MapLink\data\textures\tickets\Chiemsee Ticket_Color.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MapLink\data\textures\tickets\Ticket.rvmat"
		};
	};
	class DeerIsle_Ticket: Ticket_base
	{
		scope=2;
		displayName="Deer Isle Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		hiddenSelectionsTextures[]=
		{
			"MapLink\data\textures\tickets\Deer Isle Ticket_Color.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MapLink\data\textures\tickets\Ticket.rvmat"
		};
	};
	class Essker_Ticket: Ticket_base
	{
		scope=2;
		displayName="Essker Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		hiddenSelectionsTextures[]=
		{
			"MapLink\data\textures\tickets\Essker Ticket_Color.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MapLink\data\textures\tickets\Ticket.rvmat"
		};
	};
	class Utes_Ticket: Ticket_base
	{
		scope=2;
		displayName="Utes Ticket";
		descriptionShort="Looks like this ticket was once used to travel??";
		hiddenSelectionsTextures[]=
		{
			"MapLink\data\textures\tickets\Utes Ticket_Color.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MapLink\data\textures\tickets\Ticket.rvmat"
		};
	};
};