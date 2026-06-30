#include <M5Cardputer.h>
#include <WiFi.h>
#include <algorithm>
#include <stdexcept>

/**
 * Matriz de bytes representando a imagem monocromática da orca (64x64px).
 */
const unsigned char epd_bitmap_orca [] PROGMEM = {
	0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
	0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
	0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xc7, 0xff, 0xff, 0xff, 0xfc, 0x00, 
	0x00, 0x7f, 0xe1, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0x00, 
	0x01, 0xff, 0xe0, 0x3f, 0xc0, 0x1f, 0xff, 0x80, 0x03, 0xff, 0xf0, 0x1f, 0x00, 0x07, 0xff, 0xc0, 
	0x07, 0xff, 0xf0, 0x18, 0x7c, 0x01, 0xff, 0xe0, 0x07, 0xff, 0xf0, 0x03, 0xfe, 0x00, 0xff, 0xe0, 
	0x0f, 0xff, 0xf0, 0x07, 0xff, 0x00, 0x7f, 0xf0, 0x0f, 0xff, 0xf8, 0x07, 0xfe, 0x00, 0x3f, 0xf0, 
	0x1f, 0xff, 0xf8, 0x07, 0xfc, 0x00, 0x1f, 0xf8, 0x1f, 0xff, 0xf0, 0x07, 0x80, 0x00, 0x7f, 0xf8, 
	0x3f, 0xff, 0xf0, 0x00, 0x0f, 0x01, 0xff, 0xfc, 0x3f, 0xff, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xfc, 
	0x3f, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0xfe, 
	0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfe, 
	0x7f, 0xff, 0x00, 0x00, 0x0f, 0xf7, 0xff, 0xfe, 0x7f, 0xfe, 0x00, 0x00, 0x0f, 0xc3, 0xff, 0xfe, 
	0xff, 0xfe, 0x01, 0xf8, 0x0e, 0x01, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xf8, 0x0c, 0x01, 0xff, 0xff, 
	0xff, 0xfe, 0x0f, 0xf8, 0x0f, 0x80, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0xf8, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xfe, 0x1f, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xf8, 0x3f, 0xff, 0xff, 0xff, 
	0xff, 0xfe, 0x3f, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xf8, 0xff, 0xff, 0xff, 0xff, 
	0x7f, 0xfe, 0x3e, 0x79, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0x08, 0x37, 0xff, 0xff, 0xff, 0xfe, 
	0x7f, 0xff, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 
	0x7f, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xc0, 0x3f, 0xe1, 0xff, 0xff, 0xfc, 
	0x3f, 0xff, 0xc0, 0x1f, 0x83, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xe0, 0x1f, 0x07, 0xff, 0xff, 0xfc, 
	0x1f, 0xff, 0xf0, 0x0c, 0x07, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xf8, 0x00, 0x0f, 0xff, 0xff, 0xf8, 
	0x0f, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xf0, 
	0x07, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xe0, 
	0x03, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0x80, 
	0x00, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0xff, 0xff, 0xff, 0x00, 
	0x00, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0xff, 0xff, 0xfc, 0x00, 
	0x00, 0x0f, 0xff, 0xf8, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xf8, 0xff, 0xff, 0xe0, 0x00, 
	0x00, 0x03, 0xff, 0xfe, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00
};

/**
 * Definição dos estados da Máquina de Estados (State Machine).
 */
enum system_state_enum {
    STATE_SPLASH,
    STATE_MENU,
    STATE_AI_WIP,      // Tela de Em Construção
    STATE_WIFI_SCAN,   // Ativa a antena
    STATE_WIFI_SELECT  // Exibe a lista
};

// ==========================================
// VARIÁVEIS GLOBAIS DE CONTROLE
// ==========================================
system_state_enum current_system_state = STATE_SPLASH;
unsigned long splash_start_time = 0;
int selected_menu_index = 0;

// Variáveis do Scanner Wi-Fi
int num_networks_found = 0;
int selected_wifi_index = 0;
const int networks_per_page = 4;

// ==========================================
// FUNÇÕES DE RENDERIZAÇÃO (VIEWS)
// ==========================================

/**
 * Renderiza a Splash Screen da Orca.
 *
 * @param int screen_x Posição X.
 * @param int screen_y Posição Y.
 * @return void
 */
void render_orca_splash(int screen_x, int screen_y) {
    if (!M5Cardputer.Display.width()) throw std::runtime_error("Display falhou.");
    M5Cardputer.Display.drawBitmap(screen_x, screen_y, epd_bitmap_orca, 64, 64, TFT_WHITE, TFT_BLACK);
}

/**
 * Renderiza o Menu Principal interativo.
 *
 * @return void
 */
void render_main_menu() {
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    M5Cardputer.Display.setTextSize(2);
    
    M5Cardputer.Display.setTextColor(TFT_GREEN, TFT_BLACK);
    M5Cardputer.Display.setCursor(10, 10);
    M5Cardputer.Display.println("ORCA OS");
    M5Cardputer.Display.drawLine(10, 30, 230, 30, TFT_GREEN);
    
    // Opção 1
    M5Cardputer.Display.setCursor(10, 45);
    if (selected_menu_index == 0) M5Cardputer.Display.setTextColor(TFT_BLACK, TFT_GREEN);
    else M5Cardputer.Display.setTextColor(TFT_GREEN, TFT_BLACK);
    M5Cardputer.Display.println("> 1. IA DeepSeek");
    
    // Opção 2
    M5Cardputer.Display.setCursor(10, 70);
    if (selected_menu_index == 1) M5Cardputer.Display.setTextColor(TFT_BLACK, TFT_GREEN);
    else M5Cardputer.Display.setTextColor(TFT_GREEN, TFT_BLACK);
    M5Cardputer.Display.println("> 2. Scanner Wi-Fi");
    
    M5Cardputer.Display.setTextColor(TFT_DARKGREY, TFT_BLACK);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setCursor(10, 115);
    M5Cardputer.Display.println("Navegue: [,] e [.] | Ok: [ENTER]");
}

/**
 * Renderiza o menu paginado de redes Wi-Fi disponíveis.
 *
 * @return void
 * * Regra de negócio: Exibe máximo de 4 redes por vez para respeitar
 * os limites do display TFT.
 */
void render_wifi_menu() {
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(TFT_GREEN);
    M5Cardputer.Display.setCursor(5, 5);
    M5Cardputer.Display.println("Redes Encontradas:");
    M5Cardputer.Display.drawLine(0, 15, 240, 15, TFT_DARKGREY);

    if (num_networks_found == 0) {
        M5Cardputer.Display.println("\nNenhuma rede encontrada.");
        M5Cardputer.Display.println("Pressione [DEL] para voltar.");
        return;
    }

    int current_page = selected_wifi_index / networks_per_page;
    int start_index = current_page * networks_per_page;
    int end_index = start_index + networks_per_page;
    if (end_index > num_networks_found) end_index = num_networks_found;

    int cursor_y = 25;
    for (int i = start_index; i < end_index; ++i) {
        if (i == selected_wifi_index) M5Cardputer.Display.setTextColor(TFT_BLACK, TFT_GREEN);
        else M5Cardputer.Display.setTextColor(TFT_WHITE, TFT_BLACK);
        
        M5Cardputer.Display.setCursor(5, cursor_y);
        String ssid_name = WiFi.SSID(i);
        if (ssid_name.length() > 25) ssid_name = ssid_name.substring(0, 25) + "..."; 
        
        M5Cardputer.Display.println("> " + ssid_name);
        cursor_y += 20;
    }
    
    M5Cardputer.Display.setTextColor(TFT_DARKGREY, TFT_BLACK);
    M5Cardputer.Display.setCursor(5, 115);
    M5Cardputer.Display.printf("Voltar: [DEL] | Pagina %d/%.0f", current_page + 1, ceil((float)num_networks_found/networks_per_page));
}

// ==========================================
// CICLO DE VIDA DO HARDWARE
// ==========================================

void setup() {
    auto m5_cfg = M5.config();
    M5Cardputer.begin(m5_cfg, true);
    
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    render_orca_splash(88, 10); 
    
    M5Cardputer.Display.setTextColor(TFT_GREEN);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.setCursor(30, 85);
    M5Cardputer.Display.println("ORCA SYSTEM v1.0");
    
    splash_start_time = millis();
}

void loop() {
    M5Cardputer.update();
    
    // ESTADO: SPLASH SCREEN
    if (current_system_state == STATE_SPLASH) {
        if (millis() - splash_start_time > 3000) {
            current_system_state = STATE_MENU;
            render_main_menu();
        }
    }
    
    // ESTADO: MENU PRINCIPAL
    else if (current_system_state == STATE_MENU) {
        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState key_state = M5Cardputer.Keyboard.keysState();
            
            if (std::find(key_state.word.begin(), key_state.word.end(), '.') != key_state.word.end()) {
                if (selected_menu_index < 1) {
                    selected_menu_index++;
                    render_main_menu();
                }
            }
            else if (std::find(key_state.word.begin(), key_state.word.end(), ',') != key_state.word.end()) {
                if (selected_menu_index > 0) {
                    selected_menu_index--;
                    render_main_menu();
                }
            }
            else if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
                if (selected_menu_index == 0) {
                    current_system_state = STATE_AI_WIP;
                    M5Cardputer.Display.fillScreen(TFT_BLACK);
                    M5Cardputer.Display.setCursor(10, 40);
                    M5Cardputer.Display.setTextColor(TFT_YELLOW);
                    M5Cardputer.Display.setTextSize(2);
                    M5Cardputer.Display.println("Em Construcao!");
                    M5Cardputer.Display.setTextSize(1);
                    M5Cardputer.Display.setCursor(10, 80);
                    M5Cardputer.Display.println("Pressione [DEL] para voltar");
                } 
                else if (selected_menu_index == 1) {
                    current_system_state = STATE_WIFI_SCAN;
                }
            }
        }
    }
    
    // ESTADO: TELA DE "EM CONSTRUÇÃO"
    else if (current_system_state == STATE_AI_WIP) {
        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
            if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) { // Tecla DEL do Cardputer
                current_system_state = STATE_MENU;
                render_main_menu();
            }
        }
    }

    // ESTADO: ESCANEANDO REDES
    else if (current_system_state == STATE_WIFI_SCAN) {
        M5Cardputer.Display.fillScreen(TFT_BLACK);
        M5Cardputer.Display.setCursor(10, 40);
        M5Cardputer.Display.setTextColor(TFT_YELLOW);
        M5Cardputer.Display.setTextSize(2);
        M5Cardputer.Display.println("Escaneando...");
        
        num_networks_found = WiFi.scanNetworks();
        selected_wifi_index = 0;
        
        current_system_state = STATE_WIFI_SELECT;
        render_wifi_menu();
    }
    
    // ESTADO: LISTA DE REDES WI-FI
    else if (current_system_state == STATE_WIFI_SELECT) {
        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState key_state = M5Cardputer.Keyboard.keysState();
            
            if (std::find(key_state.word.begin(), key_state.word.end(), '.') != key_state.word.end()) {
                if (selected_wifi_index < num_networks_found - 1) {
                    selected_wifi_index++;
                    render_wifi_menu();
                }
            }
            else if (std::find(key_state.word.begin(), key_state.word.end(), ',') != key_state.word.end()) {
                if (selected_wifi_index > 0) {
                    selected_wifi_index--;
                    render_wifi_menu();
                }
            }
            else if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) {
                current_system_state = STATE_MENU;
                selected_menu_index = 1; // Volta com o cursor em cima da opção Wi-Fi
                render_main_menu();
            }
        }
    }
}