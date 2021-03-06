#include "pebble.h"

#include "Character.h"
#include "Items.h"
#include "Logging.h"
#include "Menu.h"
#include "MiniDungeon.h"
#include "Shop.h"
#include "UILayers.h"
#include "Utils.h"

#if ALLOW_SHOP

#if ALLOW_ITEM_SHOP

void ShopItemMenuAppear(Window *window);

void DrawMainItemShopWindow(void)
{
	ShowAllItemCounts();
	ShowMainWindowRow(0, "Gold", UpdateGoldText());
}

void BuyItem(ItemType type)
{
	uint8_t cost = GetItem(type)->cost;
	CharacterData *data = GetCharacter();
	if (data->gold >= cost)
	{
		if(AddItem(GetItem(type)))
		{
			data->gold -= cost;
			DrawMainItemShopWindow();
		}
	}	
}

uint8_t GetSellPrice(ItemType type) {
	uint8_t cost = GetItem(type)->cost;
	return (uint8_t)(cost * SALE_PERCENT);
}
void SellItem(ItemType type)
{
	if(!RemoveItem(GetItem(type))) {
		return;
	}
	CharacterData *data = GetCharacter();
	data->gold += GetSellPrice(type);
	DrawMainItemShopWindow();
	
}

void BuyPotion(void)
{
	BuyItem(ITEM_TYPE_POTION);
}

void BuyElixir(void)
{
	BuyItem(ITEM_TYPE_FULL_POTION);
}

void BuyBomb(void)
{
	BuyItem(ITEM_TYPE_FIRE_SCROLL);
}

void BuyIcicle(void)
{
	BuyItem(ITEM_TYPE_ICE_SCROLL);
}

void BuySpark(void)
{
	BuyItem(ITEM_TYPE_LIGHTNING_SCROLL);
}

void SellPotion(void)
{
	SellItem(ITEM_TYPE_POTION);
}

void SellElixir(void)
{
	SellItem(ITEM_TYPE_FULL_POTION);
}

void SellBomb(void)
{
	SellItem(ITEM_TYPE_FIRE_SCROLL);
}

void SellIcicle(void)
{
	SellItem(ITEM_TYPE_ICE_SCROLL);
}

void SellSpark(void)
{
	SellItem(ITEM_TYPE_LIGHTNING_SCROLL);
}

// These costs need to match the numbers in the costs array above
MenuDefinition shopItemMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "Return to shop menu", PopMenu},
		{"10", "Buy Potion", BuyPotion},
		{"100", "Buy Elixir", BuyElixir},
		{"20", "Buy Bomb", BuyBomb},
		{"20", "Buy Icicle", BuyIcicle},
		{"20", "Buy Spark", BuySpark},
	},
	.appear = ShopItemMenuAppear,
	.mainImageId = -1
};

MenuDefinition shopSellItemMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "Return to shop menu", PopMenu},
		{"2", "Sell Potion", SellPotion},
		{"20", "Sell Elixir", SellElixir},
		{"4", "Sell Bomb", SellBomb},
		{"4", "Sell Icicle", SellIcicle},
		{"4", "Sell Spark", SellSpark},
	},
	.appear = ShopItemMenuAppear,
	.mainImageId = -1
};

void ShopItemMenuAppear(Window *window)
{
	MenuAppear(window);
	DrawMainItemShopWindow();
}

void ShowShopItemMenu(void)
{
	PushNewMenu(&shopItemMenuDef);
}

void ShowShopSellItemMenu(void)
{
	PushNewMenu(&shopSellItemMenuDef);
}

#endif //ALLOW_ITEM_SHOP

#if ALLOW_STAT_SHOP

void ShopStatMenuAppear(Window *window);

static uint8_t statPointsPurchased = 0;

void ResetStatPointsPurchased(void)
{
	statPointsPurchased = 0;
}

void SetStatPointsPurchased(uint8_t count)
{
	statPointsPurchased = count;
}

uint8_t GetStatPointsPurchased(void)
{
	return statPointsPurchased;
}

const char *UpdateStatCostText(void)
{
	static char costText[] = "0000"; // Needs to be static because it's used by the system later.
	uint16_t cost = 1 << statPointsPurchased;
	UIntToString(costText, cost);
	return costText;
}

void DrawMainStatShopWindow(void)
{
	ShowMainWindowRow(0, "Gold", UpdateGoldText());
	ShowMainWindowRow(1, "Stat Point", UpdateStatCostText());
}

void BuyStatPoint(void)
{
	uint16_t cost = 1 << statPointsPurchased;
	CharacterData *data = GetCharacter();
	if (data->gold >= cost)
	{
		++statPointsPurchased;
		AddStatPointToSpend();
		data->gold -= cost;
		DrawMainStatShopWindow();
	}
}

MenuDefinition shopStatMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "Return to shop menu", PopMenu},
		{"Buy", "Buy stat point", BuyStatPoint},
	},
	.appear = ShopStatMenuAppear,
	.mainImageId = -1
};

void ShopStatMenuAppear(Window *window)
{
	MenuAppear(window);
	DrawMainStatShopWindow();
}

void ShowShopStatMenu(void)
{
	PushNewMenu(&shopStatMenuDef);
}

#endif //ALLOW_STAT_SHOP

void ShopMenuWindowAppear(Window *window);

MenuDefinition shopMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "Return to adventure", PopMenu},
#if ALLOW_ITEM_SHOP
		{"Buy", "Buy items", ShowShopItemMenu},
		{"Sell", "Sell items", ShowShopSellItemMenu},
#endif
#if ALLOW_STAT_SHOP
		{"Stats", "Buy stat points", ShowShopStatMenu},
#endif
	},
	.appear = ShopMenuWindowAppear,
	.mainImageId = RESOURCE_ID_IMAGE_SHOP
};

void ShopMenuWindowAppear(Window *window)
{
	MenuAppear(window);
	ShowMainWindowRow(0, "Shop", "");
}

void ShowShopWindow(void)
{
	INFO_LOG("Entering shop.");
	PushNewMenu(&shopMenuDef);
}
#endif //ALLOW_SHOP
