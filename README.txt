This is my final project for Advanced Programming:

Description: It is a game heavily based off of Pokemon.

Controls: WASD for movement, Space for skiping text, and mouse clicking for buttons in game.

How the game works:

    Like Pokemon, the player goes around battling and catching Pokemon. In a battle the player attack the wild pokemon, use an item (for healing), try catching the pokemon, swap out their pokemon for another one, or run away. When attacking, each attack move has a base power, but the damage delt is based of off the type of pokemon, where some types are weak against others and some types are strong against others (this will boost or weaken the attack power), and each Pokemon has their own attack multiplier that is based off of their current level (meaning a higher level will make the attack power higher as well). When trying to catch another Pokemon, the success rate will depend on the type of pokeball used (a better pokeball will have a higher chance of catching the pokemon) and the level of the wild pokemon (in relation to the player's avarage pokemon level). When it comes to running, the player will have a 50 percent chance of escaping. If the player wins a battle, the players pokemon (who have not fainted/been defeated in battle) will get experience depending on the level of the Pokemon defeated, and when enough experience has been gained, the pokemon will level up (gaining more attack and defence power, and even might learn some new moves). Also, if all the players Pokemon faint/are defeated, the player will loose the battle and be brought to the pokecenter to heal all their Pokemon.

	The player will only run into wild pokemon while in a specific quadrant of the map (when the player is not on the stone pathway), and depending on the quadrant, the wild pokemon will be a certain type (meaning that if the player is in a water-y quadrant, then the wild pokemon will be a water type pokemon). After quiting the game, a file called Your_Results.txt will be generated with stats about your playthrough, inclouding how many Pokemon you caught and what level they were.

Credits:

	Pokemon images: Stable Diffusion (AI generated)
	Music: Nintendo/Pokemon - Pokemon Diamond and Pearl and Pokemon Sword and Shield
	Other images and sound effects: Google images and Youtube and Nintendo/Pokemon
	Alexandre Tolstenko: For helping me get SDL working (CMakeLists.txt and main.cpp SDL set up)

Additional Note: I wanted to add a bunch more, like animations for battles and other functionality, as well as general cleanup, but ran out of time.