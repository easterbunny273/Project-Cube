#include "BambooEngine/include/SemanticSceneNodes/ISemanticSceneNode.h"

void ISemanticSceneNode::AddChild(ISemanticSceneNode *pNode)
{
    m_vpChildren.push_back(pNode);

    ItlTriggerOnChildAdded(pNode);
}

void ISemanticSceneNode::RemoveChild(ISemanticSceneNode *pNode)
{
    for (auto iter=m_vpChildren.begin(); iter != m_vpChildren.end(); iter++)
        if (*iter == pNode)
        {
            m_vpChildren.erase(iter);

            break;
        }

    ItlTriggerOnChildRemoved(pNode);
}

ISemanticSceneNode::t_const_children_vec *ISemanticSceneNode::GetChildren()
{
    return &m_vpChildren;
}

void ISemanticSceneNode::RegisterListener(ISemanticSceneNode::IListener *pListener)
{
    m_lpListener.push_back(pListener);
}

void ISemanticSceneNode::UnregisterListener(ISemanticSceneNode::IListener *pListener)
{
    m_lpListener.remove(pListener);
}

void ISemanticSceneNode::SetTransformMatrix(glm::mat4 mTransform)
{
    m_mTransformMatrix = mTransform;

    ItlTriggerOnTransformMatrixChanged();
}

void ISemanticSceneNode::ItlTriggerOnTransformMatrixChanged()
{
    for (auto iter = m_lpListener.begin(); iter != m_lpListener.end(); iter++)
        (*iter)->OnTransformMatrixChanged(this, m_mTransformMatrix);
}

void ISemanticSceneNode::ItlTriggerOnParametersHaveChanged()
{
    for (auto iter = m_lpListener.begin(); iter != m_lpListener.end(); iter++)
        (*iter)->OnParametersHaveChanged(this);
}

void ISemanticSceneNode::ItlTriggerOnChildAdded(ISemanticSceneNode *pNewNode)
{
    assert (pNewNode != NULL);

    for (auto iter = m_lpListener.begin(); iter != m_lpListener.end(); iter++)
        (*iter)->OnChildAdded(this, pNewNode);
}

void ISemanticSceneNode::ItlTriggerOnChildRemoved(ISemanticSceneNode *pRemovedChild)
{
    assert (pRemovedChild != NULL);

    for (auto iter = m_lpListener.begin(); iter != m_lpListener.end(); iter++)
        (*iter)->OnChildRemoved(this, pRemovedChild);
}


ISemanticSceneNode *ISemanticSceneNode::GetChild(unsigned int nIndex)
{
    assert (nIndex < GetNumChildNodes());

    return m_vpChildren[nIndex];
}


