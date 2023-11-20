#ifndef MENU_H
#define MENU_H

#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/logger.h>
#include "utils.h"

// Function declarations

void menu(libfreenect2::Freenect2Device* dev);

#endif // MENU_H