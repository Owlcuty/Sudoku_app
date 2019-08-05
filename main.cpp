#include <stdio.h>
#include <cstring>
#include <assert.h>

#include <SFML/Graphics.hpp>

typedef sf::Vector2f Size;
typedef sf::Vector2f Coord;

enum Btn_classes {
	Btn, BtnMode, BtnGroup, BtnSudoku
};

namespace Settings {
	const int ReferenceObject = 0;

	const int NMax = 9;
	const int Width = 1366;
	const int Height = 768;

	const sf::Color Salmon     = sf::Color(250, 128, 114);
	const sf::Color Orange     = sf::Color(255, 165, 0);
	const sf::Color DarkKhaki  = sf::Color(189, 183, 107);
	const sf::Color Aquamarine = sf::Color(127, 255, 212);
	const sf::Color Maroon     = sf::Color(128, 0,   0);
	const sf::Color Plum       = sf::Color(221, 160, 221);
	const sf::Color Peach      = sf::Color(255, 204, 153);
	const sf::Color LightPeach = sf::Color(254, 214, 188);
	const sf::Color PastelSky  = sf::Color(222, 247, 254);
	const sf::Color Salad      = sf::Color(195, 251, 216);
	const sf::Color Purple     = sf::Color(198, 216, 255);
	const sf::Color Carmine    = sf::Color(150, 0,   24);
	
	const Size Btn_dflt_size    = sf::Vector2f(160, 30);
	const Size Btn_sudoku_size  = sf::Vector2f(60,  60);
	const Size Btn_grp_size     = sf::Vector2f(60,  30);
	const Size Mini_sudoku_size = sf::Vector2f(30,  30);
	const sf::Color Btn_dflt_color = sf::Color(230, 230, 230);

	const int Num_back_parts = 13;
	const sf::Color Background_col[Num_back_parts] = {sf::Color(227, 175, 196),
					      		  sf::Color(227, 176, 191),
					     		  sf::Color(228, 178, 186),
					     		  sf::Color(229, 179, 182),
					     		  sf::Color(229, 183, 180),
					     		  sf::Color(230, 190, 182),
					     		  sf::Color(231, 197, 183),
					     		  sf::Color(231, 203, 184),
					     		  sf::Color(232, 210, 185),
					     		  sf::Color(233, 216, 187),
					      		  sf::Color(233, 222, 188),
					     		  sf::Color(234, 228, 189),
					     		  sf::Color(235, 234, 191)};

	const sf::Color Btn_empty_col  = sf::Color(236, 240, 208);
	const sf::Color Colors_groups[NMax] = {Salmon,
					       PastelSky,
					       DarkKhaki,
					       Purple,
					       Peach,
					       Salad,
					       Orange,
					       Aquamarine,
					       Maroon};

	const int Text_size = 18;
	const sf::Color Text_color = sf::Color::Black;

	const int Warn_size = 21;
	const sf::Color Warn_color= Carmine;

	const int Sdk_size = 26;

	const int Title_size = 24;

	const int Outline_size = 2;
	const int Outline_sdk_size = 1;
	const sf::Color Outline_color = sf::Color::Black;

	const int Num_btn_menu = 3;

	const int Classic_map[NMax][NMax] = {{1, 1, 1, 2, 2, 2, 3, 3, 3},
					     {1, 1, 1, 2, 2, 2, 3, 3, 3},
					     {1, 1, 1, 2, 2, 2, 3, 3, 3},
					     {4, 4, 4, 5, 5, 5, 6, 6, 6},
					     {4, 4, 4, 5, 5, 5, 6, 6, 6},
					     {4, 4, 4, 5, 5, 5, 6, 6, 6},
					     {7, 7, 7, 8, 8, 8, 9, 9, 9},
					     {7, 7, 7, 8, 8, 8, 9, 9, 9},
					     {7, 7, 7, 8, 8, 8, 9, 9, 9}};
}

void rand_array(int nmax, int array[]);
void copy_array(int ar1[], const int ar2[], int n);

bool is_clicked(sf::Clock clock);

// ----- Background ----------------------------------------------------------------------------------------------------

class Background {

   public:
	Background() {

		for (int part = 0; part < Settings::Num_back_parts; part ++) {
			parts[part].setSize(sf::Vector2f(Settings::Width / 13.0 * (part + 1), Settings::Height));
			parts[part].setFillColor(Settings::Background_col[part]);
		}
	}

	void draw(sf::RenderWindow* window) {
		for (int part = Settings::Num_back_parts - 1; part >= 0; part--) {
			window->draw(parts[part]);
		}
	}

   private:
	sf::RectangleShape parts[Settings::Num_back_parts];

};

// ---------------------------------------------------------------------------------------------------------------------

// ----- Text ----------------------------------------------------------------------------------------------------------

struct Text {
    Coord pos;
    std::string str;
    sf::Font font;
    sf::Color color;
    int char_size;

    int angle = 0;

    void draw(sf::RenderWindow *window);
};


void Text::draw(sf::RenderWindow *window) {
    assert(0 <= pos.x && pos.x < Settings::Width);
    assert(0 <= pos.y && pos.y < Settings::Height);
    assert(char_size > 10 && char_size < 36);

    sf::Text text;

    text.setFont(font);
    text.setString(str);
    text.setFillColor(color);
    text.setCharacterSize(char_size);
    text.setPosition(pos);
    text.rotate(angle);

    window->draw(text);
}

void set_std_text(Text* text, Coord pos, std::string str, sf::Font font) {
	*text = {pos, str, font, Settings::Text_color, Settings::Text_size};
}

void set_title(Text* text, Coord pos, std::string str, sf::Font font) {
	*text = {pos, str, font, Settings::Text_color, Settings::Title_size};
}

void set_warning_text(Text* text, Coord pos, std::string str, sf::Font font) {
	*text = {pos, str, font, Settings::Warn_color, Settings::Warn_size};
}
// ---------------------------------------------------------------------------------------------------------------------


// ----- New Button Class ----------------------------------------------------------------------------------------------

class Button {

   public:

        int id;

	bool is_active;

	Coord pos;

	int number;

	sf::Font font;
	Text text;

	Size size;
	sf::Color fill_color;
	
	int outline_size = Settings::Outline_size;

	Button(int id, Coord pos, sf::Font font, bool is_active = false) {
		this->id = id;
		this->pos = pos;
		this->font = font;
		this->is_active = is_active;
	}

	virtual Button* clone() const;

	void draw(sf::RenderWindow* window) {
		assert(0 <= pos.x && pos.x <= Settings::Width);
		assert(0 <= pos.y && pos.y <= Settings::Height);

		sf::RectangleShape shape;

		shape.setPosition(pos);
		shape.setSize(size);
		shape.setFillColor(fill_color);
		shape.setOutlineColor(sf::Color(10, 10, 10));
		shape.setOutlineThickness(outline_size);

		window->draw(shape);
		text.draw(window);
	}

};

Button *Button::clone() const {
	return new Button(*this);
}


class Btn_mode : public Button {

   public:
	
	Btn_mode(int id, Coord pos, sf::Font font, std::string str) : Button(id, pos, font)
	{
	     size = Settings::Btn_dflt_size;
	     fill_color = Settings::Btn_dflt_color;
	     this->text = {{pos.x + 80 - sizeof(str), pos.y + 3}, str, font, Settings::Text_color, Settings::Text_size};
	}

	int clicked(int id) {
		return (id == 1);
	}

	Button* clone() const override {
		return new Btn_mode(*this);
	}
};

class Btn_group: public Button {

   public:
	
	Btn_group(int id, Coord pos, sf::Font font, int number) : Button(id, pos, font)
	{
	     assert(isdigit(number));
	     size = Settings::Btn_grp_size;
	     fill_color = Settings::Colors_groups[number - 1];
	     this->number = number;
	     text = {{pos.x + 10, pos.y + 3}, std::to_string(number), font, Settings::Text_color, Settings::Text_size};
	}

	int clicked(int id) {
		return (id == 1);
	}

	Button* clone() const override {
		return new Btn_group(*this);
	}
};

class Btn_sudoku : public Button {

   public:
	
	Btn_sudoku(int id, Coord pos, sf::Font font) : Button(id, pos, font)
	{
	     size = Settings::Btn_sudoku_size;
	     fill_color = Settings::Btn_empty_col;
	     outline_size = Settings::Outline_sdk_size;
	     this->text = {{pos.x + 24, pos.y + 14}, "", font, Settings::Text_color, Settings::Sdk_size};
	}

	void set_color(int number) {
	     fill_color = Settings::Colors_groups[number - 1];
	}

	int clicked(int id) {
	     return (id == 1);
	}
	
	Button* clone() const override {
		return new Btn_sudoku(*this);
	}

};

// --------------------------------------------------------------------------------------------------------------------


/*
// ----- Button's engine -----------------------------------------------------------------------------------------------

struct Button {

    void (*clicked)();

    std::string text;

    int id;

    bool is_active = false;

    Coord pos;
    int number;

    sf::Font font;

    Size size = Settings::Btn_size;
    sf::Color fill_col = Settings::Btn_fill;

    int outline_size = Settings::Outline_size;


    //-----------------------------

    sf::Color outline_color;

    Text fill_text;

    //-----------------------------

    void draw(sf::RenderWindow *window);
};

void Button::draw(sf::RenderWindow *window) {
    assert(0 <= pos.x && point.x < Settings::Width);
    assert(0 <= pos.y && point.y < Settings::Height);

    outline_color = Settings::Func_graph_color[(int) (is_active) * number];
    if (id == 6) {
        outline_color = Settings::Fill_act_col[(int) (is_active) * number];
    }
    fill_text = {{pos.x + 10, point.y + 3}, text, font, Settings::Text_color, Settings::Text_size};

    sf::RectangleShape shape;

    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(fill_col);
    shape.setOutlineColor(outline_color);
    shape.setOutlineThickness(outline_size);

    window->draw(shape);
    fill_text.draw(window);
}

void draw_buttons(Button buttons[], int num_of_elems, sf::RenderWindow *window) {
    assert(0 < num_of_elems && num_of_elems <= Settings::NMax_for_arrays);

    for (int i = 1; i < num_of_elems; i++) {
        buttons[i].draw(window);
    }
}

void set_buttons(Button buttons[], int num_of_elems, int step_x, int step_y) {
    sf::Font font;
    sf::Color fill_col;
    Coord start_pos;
    int outline_size = 0;

    for (int num = 0; num < num_of_elems; num++) {
        if (!num) {
            font = buttons[0].font;
            fill_col = buttons[num].fill_col;
            start_pos = buttons[num].pos;
            outline_size = buttons[num].outline_size;
            buttons[num].pos = {0, 0};
            buttons[num].size = {0, 0};
            continue;
        }
        buttons[num].pos = {start_pos.x + step_x * (num - 1), start_pos.y + step_y * (num - 1)};
        buttons[num].font = font;
        buttons[num].fill_col = fill_col;
        buttons[num].outline_size = outline_size;
        buttons[num].number = num;
    }
}

bool check_border(const Button &border, sf::RenderWindow *window);

*/

int empty() {

	return 0;
}

void empty_gr(sf::Color* color, sf::Color from) {

}

void empty_color(sf::Color color, Button* btn) {

}

void fill_color(sf::Color color, Button* btn) {

}

void draw_buttons(sf::RenderWindow* window, Button** btns, int num_elem) {
	for (int i = 0; i < num_elem; i ++) {
		btns[i]->draw(window);
	}
}

void draw_mtrx_buttons(sf::RenderWindow* window, Button* btns[][Settings::NMax], int N) {
	for (int x = 0; x < N; x ++) {
		draw_buttons(window, btns[x], N);
	}
}

void build_menu_window(sf::RenderWindow* window, Text* text_title, Button** choose, Text* text_choose, sf::Font font) {
	
	set_title(text_title, {Settings::Width / 2 - 35, Settings::Height / 2 - 70}, "Menu", font);
	text_title->draw(window);

	draw_buttons(window, choose, Settings::Num_btn_menu);
	set_std_text(text_choose, {Settings::Width / 2 - 300, Settings::Height - 140}, 
			"Classic - standart 3x3 square groups. Custom - you choose the shapes of groups", font);
	text_choose->draw(window);

}

void build_sudoku(sf::RenderWindow* window, Button* sudoku[][Settings::NMax], sf::Font font, int groups_map[][Settings::NMax]) {

	sf::RectangleShape border;
	border.setPosition(sf::Vector2f(0.3 * Settings::Width, 0.1484375 * Settings::Height));
	border.setSize(sf::Vector2f(60 * Settings::NMax, 60 * Settings::NMax));
	border.setOutlineColor(sf::Color(10, 10, 10));
	border.setOutlineThickness(Settings::Outline_size);

	window->draw(border);

	for (int y = 0; y < Settings::NMax; y ++) {
		for (int x = 0; x < Settings::NMax; x ++) {
			sudoku[y][x]->fill_color = Settings::Colors_groups[groups_map[y][x] - 1];
		}
	}

	draw_mtrx_buttons(window, sudoku, Settings::NMax);

}

/*
void set_btn(Button** btn, int* id, Coord pos, sf::Font font, int type, std::string text = nullptr, int number = -1) {

	switch (type) {
		case Btn:
			*btn = new Button((*id)++, pos, font);
			break;
		case BtnMode:
			*btn = new Btn_mode((*id)++, pos, font, text);
			break;
		case BtnGroup:
			*btn = new Btn_group((*id)++, pos, font, number);
			break;
		case BtnSudoku:
			printf("%i\n", __LINE__);
			*btn = new Btn_sudoku((*id)++, pos, font);
			printf("%i\n", __LINE__);
			break;
		default:
			printf("You haven't neccessary buttons for this mode\n");
			break;
	}	
	
}
*/

void set_buttons(Button** btns, int num_elem, int* id, sf::Vector2f step, std::string texts[] = nullptr, int number = -1) {
	for (int i = 1; i < num_elem; i ++) {
		btns[i] = btns[Settings::ReferenceObject]->clone();
		btns[i]->pos = {btns[i]->pos.x + step.x * i, btns[i]->pos.y + step.y * i}; 
		btns[i]->id = (*id)++;
		btns[i]->text.pos = {btns[i]->text.pos.x + step.x * i, btns[i]->text.pos.y + step.y * i};
		if (texts == nullptr) { continue; btns[i]->text.str = ""; }
		btns[i]->text.str = texts[i];
	}
}

void set_mtrx_buttons(Button* btns[][Settings::NMax], int num_elem, int* id, sf::Vector2f step, std::string texts[][Settings::NMax] = nullptr, int number = -1) {

	for (int y = 0; y < num_elem; y ++) {
		for (int x = 0; x < num_elem; x ++) {
			if (!y && !x) continue;
			btns[y][x] = btns[Settings::ReferenceObject][Settings::ReferenceObject]->clone();
			btns[y][x]->pos = {btns[y][x]->pos.x + step.x * x, btns[y][x]->pos.y + step.y * y};
			btns[y][x]->id = (*id)++;
			btns[y][x]->text.pos = {btns[y][x]->text.pos.x + step.x * x, btns[y][x]->text.pos.y + step.y * y};
			if (texts == nullptr) continue;
			btns[y][x]->text.str = texts[y][x];
		}
	}	

}

bool check_border(Button* border, sf::RenderWindow *window) {
    return ((*border).pos.x <= sf::Mouse::getPosition(*window).x
            && sf::Mouse::getPosition(*window).x <= (*border).pos.x + (*border).size.x
            && (*border).pos.y <= sf::Mouse::getPosition(*window).y
            && sf::Mouse::getPosition(*window).y <= (*border).pos.y + (*border).size.y);
}

bool is_clicked(Button* border, sf::RenderWindow *window, sf::Clock clock) {
    return (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > 200 && check_border(border, window));
}

char* itos(int n) {
	char* s;
	while (n > 0) {
		s += (n % 10) + '0';
		n /= 10;
	}
	return s;
}

int main() {
	sf::RenderWindow window(sf::VideoMode(Settings::Width, Settings::Height), "Sudoku solving");
	sf::Font font;

	if (!font.loadFromFile("Roboto-Regular.ttf"))
		return 1;


	Background* bkg = new Background();

	int last_id = 0;

	Text text_choose_menu;
	Text text_title_menu;

	Button* menu[Settings::Num_btn_menu]; //Btn_mode
	std::string menu_texts[Settings::Num_btn_menu] = {"Classic", "Custom", "Just quit"};

	menu[Settings::ReferenceObject] = new Btn_mode(last_id++, {Settings::Width / 2 - 80, Settings::Height / 2}, font, menu_texts[Settings::ReferenceObject]);

	set_buttons(menu, Settings::Num_btn_menu, &last_id, {0, 75}, menu_texts);

	int (*groups_map)[Settings::NMax] = static_cast<int (*)[Settings::NMax]>(calloc(Settings::NMax, Settings::NMax * sizeof(*groups_map)));

	Button* sudoku[Settings::NMax][Settings::NMax]; //Btn_sudoku

	sudoku[Settings::ReferenceObject][Settings::ReferenceObject] = new Btn_sudoku(last_id++, {0.3 * Settings::Width, 0.1484375 * Settings::Height}, font);

	set_mtrx_buttons(sudoku, Settings::NMax, &last_id, {60, 60});

	int is_chosen = 0;
	bool is_act = false;
	sf::Vector2i choise_now = {0, 0};

	sf::Clock clock;

	while (window.isOpen()) {
		sf::Event event{};
		while(window.pollEvent(event)) 
			if (event.type == sf::Event::Closed)
				window.close();


		window.clear(sf::Color::White);
	
		bkg->draw(&window);

		if (!is_chosen) {
			build_menu_window(&window, &text_title_menu, menu, &text_choose_menu, font);
			for (int i = 0; i < Settings::Num_btn_menu; i ++) {
			     if (is_clicked(menu[i], &window, clock)) {
				 clock.restart();
				 if (i == Settings::Num_btn_menu - 1) {
					 window.close();
				 }
				 is_chosen = i + 1;
			     }
			}

		}
		if (is_chosen == 1) {
			for (int y = 0; y < Settings::NMax; y ++) {
				for (int x = 0; x < Settings::NMax; x ++) {
					groups_map[y][x] = Settings::Classic_map[y][x];
				}
			}
			build_sudoku(&window, sudoku, font, groups_map);
			for (int y = 0; y < Settings::NMax; y ++) {
			     for (int x = 0; x < Settings::NMax; x ++) {
			          if (is_clicked(sudoku[y][x], &window, clock)) {
					clock.restart();
					if (is_act) {
						if (choise_now.x == x && choise_now.y == y) {
						       is_act = false;
						} else {
							choise_now = {x, y};
						}
						break;		
					} else {
						is_act = true;
						choise_now = {x, y};
					}
				  }
			     }
			}
			if (is_act && event.type == sf::Event::KeyReleased && 27 <= event.key.code && event.key.code <= 35) {
				sudoku[choise_now.y][choise_now.x]->text.str = (std::to_string(event.key.code - 26)).c_str();
				is_act = false;
			}
			if (is_act && event.key.code == sf::Keyboard::Escape) {
				is_act = false;
			}
			if (is_act && event.key.code == sf::Keyboard::Delete) {
				sudoku[choise_now.y][choise_now.x]->text.str = "";
			}
		}	

		window.display();

	}		

	free(groups_map);
	return 0;

}
