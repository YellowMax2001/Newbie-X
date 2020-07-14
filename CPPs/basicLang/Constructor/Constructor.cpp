#include <Constructor.h>

using namespace NewbieC;

Pipeline::Pipeline(std::string pipelineName)
{
    m_pipelineName = pipelineName;

    std::cout << "pipeline name is:" << m_pipelineName << std::endl;
    printf("Input method param:%p, set param:%p.\n", pipelineName.c_str(), m_pipelineName.c_str());
}

Pipeline::~Pipeline()
{
    ;
}


int Pipeline::AddOneNode(NodeInstanceMap nodeIns)
{
    m_nodeList.push_back(nodeIns);
    m_nodeCount++;

    std::cout<<"Add one node, type:" << nodeIns.nodeType <<", id:" << nodeIns.nodeId << ", nodeCount:" << m_nodeCount << std::endl;
}

int main(void)
{
    Pipeline* pipelineCamera = new Pipeline("pipelineCamera");

    NodeInstanceMap nodeIns = {0, PipelineNodeType::NodeCameraCapture};


    pipelineCamera->AddOneNode(nodeIns);

    delete pipelineCamera;

    return 0;
}
