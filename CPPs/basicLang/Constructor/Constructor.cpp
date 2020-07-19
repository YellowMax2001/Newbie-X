#include <Constructor.h>

using namespace NewbieC;

Pipeline::Pipeline(std::string pipelineName)
{
    m_pipelineName      = pipelineName;
    m_nodeCount         = 0;
    m_pPipelineNodesRef = NULL;

    if (NULL == m_pPipelineNodesRef)
    {
        m_pPipelineNodesRef = new int[10];
        printf("Pipeline nodes ref[%p].\n", m_pPipelineNodesRef);
    }

    std::cout << "pipeline name is:" << m_pipelineName << std::endl;
    printf("Input string[%p], name is:%s/%p.\n",
        &pipelineName, pipelineName.c_str(), m_pipelineName.c_str());
}

Pipeline::~Pipeline()
{
    if (NULL != m_pPipelineNodesRef)
    {
        printf("Delete pipeline nodes ref[%p].\n", m_pPipelineNodesRef);
        delete m_pPipelineNodesRef;
    }
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

    Pipeline pipelineSensor("pipelineSensor");

    NodeInstanceMap nodeIns = {0, PipelineNodeType::NodeCameraCapture};
    pipelineSensor.AddOneNode(nodeIns);

    Pipeline pipelineSensorCopy("pipelineSensorCopy");

    pipelineSensorCopy = pipelineSensor;

    pipelineSensorCopy.GetAndPrintPipelineName();
    pipelineSensor.AddOneNode(nodeIns);

    delete pipelineCamera;

    return 0;
}

