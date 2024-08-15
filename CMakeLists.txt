# Adafruit GFX Library
# https://github.com/terrabox/Adafruit-GFX-Library
# BSD License

cmake_minimum_required(VERSION 3.5)

idf_component_register(SRCS 
	"BarWidget.cpp" 
	"ButtonWidget.cpp" 
	"KeyboardCodes.cpp" 
	"LabelWidget.cpp" 
	"LevelIndicatorWidget.cpp" 
	"Levels.cpp" 
	"RectangleWidget.cpp" 
	"SoftKeyboardWidget.cpp" 
	"SoftKeyWidget.cpp" 
	"Widget.cpp" 
         INCLUDE_DIRS "."
         REQUIRES arduino Adafruit_BusIO)

project(TerraBox_LevelIndicator)
