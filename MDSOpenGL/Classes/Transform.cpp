#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CTransform::CTransform()
{
    m_v3Position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_fquatRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_v3Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    UpdateModelMatrix();
}

void CTransform::UpdateModelMatrix()
{
    m_mat4Model = glm::scale(glm::mat4x4(1), m_v3Scale);
    m_mat4Model = glm::translate(m_mat4Model, m_v3Position);
    m_mat4Model *= glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), Forward(), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4x4 CTransform::GetModel() const
{
    return m_mat4Model;
}

const glm::vec3 CTransform::GetPosition() const
{
    return m_v3Position;
}
void CTransform::SetPosition(const glm::vec3 _v3Position)
{
    m_v3Position = _v3Position;
    UpdateModelMatrix();
}

void CTransform::Move(const glm::vec3 _v3Offset)
{
    m_v3Position += _v3Offset;
    UpdateModelMatrix();
}

const glm::quat CTransform::GetRotation() const
{
    return m_fquatRotation;
}
void CTransform::SetRotation(const glm::fquat _fquatRotation)
{
    m_fquatRotation = _fquatRotation;
    UpdateModelMatrix();
}

void CTransform::Rotate(const glm::fquat _fquatOffset)
{
    m_fquatRotation *= _fquatOffset;
}

const glm::vec3 CTransform::GetRotationEuler(bool _bInDegrees/* = true*/) const
{
    glm::vec3 v3Output = glm::eulerAngles(m_fquatRotation);
    if (_bInDegrees) v3Output = glm::degrees(v3Output);

    return v3Output;
}
void CTransform::SetRotationEuler(const glm::vec3 _v3Rotation, bool _bInDegrees/* = true*/)
{
    m_fquatRotation = _bInDegrees ? glm::quat(glm::radians(_v3Rotation)) : glm::quat(_v3Rotation);
    UpdateModelMatrix();
}

void CTransform::RotateEuler(const glm::vec3 _v3Offset, bool _bInDegrees)
{
    glm::quat fquatOffset = _bInDegrees ? glm::quat(glm::radians(_v3Offset)) : glm::quat(_v3Offset);
    m_fquatRotation *= fquatOffset;
    UpdateModelMatrix();
}

void CTransform::LookAt(const glm::vec3 _v3Target)
{
    m_fquatRotation = glm::quatLookAt(_v3Target - m_v3Position, glm::vec3(0.0f, 1.0f, 0.0f));
    UpdateModelMatrix();
}

const glm::vec3 CTransform::GetScale() const
{
    return m_v3Scale;
}
void CTransform::SetScale(const glm::vec3 _v3Scale)
{
    m_v3Scale = _v3Scale;
    UpdateModelMatrix();
}

const glm::vec3 CTransform::Up() const
{
    return m_fquatRotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

const glm::vec3 CTransform::Forward() const
{
    return m_fquatRotation * glm::vec3(0.0f, 0.0f, -1.0f);
}

const glm::vec3 CTransform::Right() const
{
    return m_fquatRotation * glm::vec3(1.0f, 0.0f, 0.0f);
}