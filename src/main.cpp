#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define LCD_COL 16
#define LCD_ROW 2
#define PAGE_COUNT 6

LiquidCrystal_I2C lcd(0x27, LCD_COL, LCD_ROW); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
	lcd.init();
	lcd.backlight();
	Serial.begin(9600);
}

int col = 0, row = 0;
char page_buffer[PAGE_COUNT][LCD_ROW][LCD_COL] = {0};
int page = 0;
int col_buffer[PAGE_COUNT] = {0},
	row_buffer[PAGE_COUNT] = {0};

void loop() {
	if (Serial.available()) {
		char key = Serial.read();
		if (key == '\n') {
			return;
		} else if (key == '\b') {
			if (col == 0 && row == 0) {
				col = LCD_COL - 1;
				row = LCD_ROW - 1;
			} else if (col == 0 && row > 0) {
				col = LCD_COL - 1;
				row--;
			} else if (col > 0) {
				col--;
			}
			lcd.setCursor(col, row);
			lcd.write(' ');
			page_buffer[page][row][col] = ' ';
		} else if (key == '\r') {
			col_buffer[page] = col;
			row_buffer[page] = row;
			if (++page >= PAGE_COUNT) page = 0;
			lcd.clear();
			for (int i = 0; i < LCD_ROW; i++) {
				for (int j = 0; j < LCD_COL; j++) {
					key = page_buffer[page][i][j];
					if (key) {
						lcd.setCursor(j, i);
						lcd.write(key);
					}
				}
			}
			col = col_buffer[page];
			row = row_buffer[page];
			lcd.setCursor(0, 0);
		} else if (key == 27) {
			col = 0;
			row = 0;
			lcd.clear();
			memset(page_buffer[page], 0, LCD_ROW * LCD_COL * sizeof(char));
		} else {
			lcd.setCursor(col, row);
			if (col == 0 && row == 0) {
				lcd.clear();
				memset(page_buffer[page], 0, LCD_ROW * LCD_COL * sizeof(char));
			}
			lcd.write(key);
			page_buffer[page][row][col] = key;
			if (++col >= LCD_COL) {
				col = 0;
				row++;
			}
			if (row >= LCD_ROW) {
				row = 0;
			}
		}
	}
}