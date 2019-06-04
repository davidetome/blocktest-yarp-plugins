/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @author Andrea Ruzzenenti <andrea.ruzzenenti@iit.it>
 */

#include "actionYarp.h"
#include "yarp/dev/IFrameSource.h"
#include "yarpActionDepotStart.h"

using namespace yarp::dev;

namespace YarpActions
{
/**
 * @class ActionCheckJointPosition
 * @brief ...
 * @todo missing brief and detailed description
 */
class OpenFrameTransform : public ActionYarp
{
public:
    OpenFrameTransform(const CommandAttributes& parameters, const std::string& testCode) : ActionYarp(parameters, testCode), parameters_(parameters) {};
        bool execute(unsigned int testrepetition) override;

protected:
    IFrameSource*     frame_;
    CommandAttributes parameters_;
};

bool OpenFrameTransform::execute(unsigned int testrepetition)
{
    std::string tag;
    Action::getCommandAttribute(parameters_, "polydrivertag", tag);
    if (YarpActionDepotStart::polyDriverDepot_.find(tag) == YarpActionDepotStart::polyDriverDepot_.end())
    {
        auto error = "polydriver not found";
        TXLOG(Severity::error) << error << std::endl;
        addProblem(testrepetition, Severity::critical, error);
        return false;
    }

    auto pdr = YarpActionDepotStart::polyDriverDepot_[tag];
    if (!pdr->view(frame_))
    {
        auto error = "view not succeded";
        TXLOG(Severity::error) << error << std::endl;
        addProblem(testrepetition, Severity::critical, error);
        return false;
    }
    return true;
}

class ActionGetTransform : public OpenFrameTransform
{
public:
    ActionGetTransform(const CommandAttributes& parameters, const std::string& testCode) : OpenFrameTransform(parameters, testCode) {}

    bool execute(unsigned int testrepetition) override
    {
        if(!ActionGetTransform::execute(testrepetition))
            return false;
        std::string frameId, parent;
        Action::getCommandAttribute(parameters_, "frameid", frameId);
        Action::getCommandAttribute(parameters_, "parent", parent);
        auto f = frame_->getTransform(frameId, parent);
        if (!f.valid)
        {
            addProblem(testrepetition, Severity::critical, "transform not found");
            return false;
        }
        TXLOG(Severity::plot) << f.value.toString() << std::endl;
    }

    ACTIONREGISTER_DEC_TYPE(ActionGetTransform)
};

ACTIONREGISTER_DEF_TYPE(ActionGetTransform, "yarpgettransform");
}