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
        printf("This pipeline[%p] name is:%s/%p\n", &m_pipelineName,
            m_pipelineName.c_str(), m_pipelineName.c_str());

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
	int* m_pPipelineNodesRef;
};

}

#endif // End of _CONSTRUCTOR_H_