TEST_DIR=./tests
CC=g++


all: blackjack tests



blackjack: main.cpp card.hpp card_deck.hpp game.hpp player.hpp input_helper.hpp
	$(CC) main.cpp -o blackjack

tests: test_card test_card_deck test_player test_game test_input_helper

test_card: $(TEST_DIR)/test_card.cpp
	$(CC) $< -o $(TEST_DIR)/$@

test_card_deck: $(TEST_DIR)/test_card_deck.cpp
	$(CC) $< -o $(TEST_DIR)/$@

test_player: $(TEST_DIR)/test_player.cpp 
	$(CC) $< -o $(TEST_DIR)/$@

test_game: $(TEST_DIR)/test_game.cpp 
	$(CC) $< -o $(TEST_DIR)/$@

test_input_helper: $(TEST_DIR)/test_input_helper.cpp 
	$(CC) $< -o $(TEST_DIR)/$@

clean:
	-rm blackjack
	-rm $(TEST_DIR)/test_card
	-rm $(TEST_DIR)/test_card_deck
	-rm $(TEST_DIR)/test_player
	-rm $(TEST_DIR)/test_game
	-rm $(TEST_DIR)/test_input_helper

