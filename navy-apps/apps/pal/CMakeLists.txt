project(pal)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
  ./src/device/palette.c
  ./src/device/video.c
  ./src/device/input.c
  ./src/ui/magicmenu.c
  ./src/ui/font.c
  ./src/ui/uibattle.c
  ./src/ui/text.c
  ./src/ui/uigame.c
  ./src/ui/ui.c
  ./src/ui/itemmenu.c
  ./src/unuse/sound.c
  ./src/unuse/midi.c
  ./src/unuse/rixplay.c
  ./src/unuse/sdl.c
  ./src/battle/battle.c
  ./src/battle/fight.c
  ./src/FLOAT/FLOAT.c
  ./src/hal/hal.c
  ./src/scene/scene.c
  ./src/scene/map.c
  ./src/game/game.c
  ./src/game/script.c
  ./src/game/play.c
  ./src/pal-main.c
  ./src/misc/ending.c
  ./src/misc/yj1.c
  ./src/misc/rngplay.c
  ./src/misc/util.c
  ./src/global/res.c
  ./src/global/global.c
  ./src/global/palcommon.c
  ./src/main.c
  )

target_link_libraries(${PROJECT_NAME}
  PRIVATE sdl)

target_include_directories(${PROJECT_NAME}
  PRIVATE include)
