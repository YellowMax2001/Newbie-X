#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

#include <iostream>
#include <string>
#include <array>
#include <list>

namespace NewbieC {

enum PipelineNodeType
{
    NodeCameraInput,
    NodeCameraAnalysis,
    NodeCameraCapture,
    NodeCameraDisplay,
    NodeCameraMax
};

struct NodeInstanceMap
{
    int nodeId;
    PipelineNodeType nodeType;
};

class Pipeline {
public:
    Pipeline(std::string pipelineName);

    ~Pipeline();

    inline std::string* GetAndPrintPipelineName()
    {
        std::cout<<"This pipeline name is:%s"<<m_pipelineName;

        return &m_pipelineName;
    }

    int AddOneNode(NodeInstanceMap nodeIns);

    int m_nodeCount;
    std::list<NodeInstanceMap> m_nodeList;
protected:
    // This will delete the default copy constructor, better use it.
    //Pipeline (const Pipeline& pipeline) = delete;
    //Pipeline& operator=(const Pipeline& pipeline) = delete;

private:
    // Private copy constructor, if use copy constructor,
    // compiler will drop a error.
    //Pipeline(const Pipeline& pipeline);
	std::string m_pipelineName;
};

}

#endif