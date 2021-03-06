/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

 /**
  * @file actionCanWrite.h
  * @author Davide Tomé <davide.tome@iit.it>
  */

#pragma once

#include <actionYarp.h>

namespace YarpActions
{

class ActionCanWrite : public ActionYarp
{
public:
    ActionCanWrite(const CommandAttributes& commandAttributes, const std::string& testCode);
    execution execute(const TestRepetitions& testrepetition) override;
    void beforeExecute() override;

protected:
	std::string messageId_{""};
  std::string message_{""};

	ACTIONREGISTER_DEC_TYPE(ActionCanWrite)
};

} // namespace YarpActions
