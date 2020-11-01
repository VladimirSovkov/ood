#pragma once
#include<functional>
#include <memory>
#include "ICommand.h"

typedef std::function<void(std::unique_ptr<ICommand>&& commandPtr)> CommandFunc;