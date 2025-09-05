#include "stdafx.h"
#include "camera.h"
#include "EventManager.h"
#include "Render/Vulkan/vulkanControl.h"
#include "Render/Renderer.h"


namespace StudyEngine {

	Camera::Camera()
	{
		VkDeviceSize cameraSize = sizeof(CameraBuffer);

		cameraBuffer.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			cameraBuffer[i] = new VulkanUniformBuffer(cameraSize);
		
		}
		updateDescriptorSets();
		worldToCamMatrix = Matrix4::CreateTranslation(Vector3(0.0f, 1.0f, 0.0f)) * Matrix4::CreateRotationY(Math::Pi / 4);
		yaw = Math::ToDegrees(Math::Pi / 4);

		worldToCamMatrix.Invert();
		projectionMatrix = Matrix4::CreatePerspectiveFOV(fov, float(WIDTH), (float)HEIGHT, near, far);
	}

	Camera::~Camera()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			delete cameraBuffer[i];
		}
	}

	void Camera::AddMovement(Vector3 dir)
	{
		movementInput += dir;
	}

	void Camera::RotateCamera(float xoffset, float yoffset)
	{
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		if (yaw > 180.0f)
			yaw = 180.f;
		if (yaw < -180.0f)
			yaw = -180.f;
		//std::cout << "new Yaw" << yaw << std::endl;
		//std::cout << "new pitch" << pitch << std::endl;
		Vector3 newFront;
		newFront.x = -sinf(Math::ToRadians(yaw)) * cosf(Math::ToRadians(pitch));
		newFront.y = -sinf(Math::ToRadians(pitch));
		newFront.z = cosf(Math::ToRadians(yaw)) * cosf(Math::ToRadians(pitch));
		newFront.Normalize();
		Matrix4 camToWorldMat = worldToCamMatrix;
		camToWorldMat.Invert();

		Vector3 worldUp = Vector3(0.f, 1.f, 0.f);
		Vector3 newRight = Vector3::Cross(newFront, worldUp);
		Vector3 newUp = Vector3::Cross(newRight, newFront);

		Matrix4 newLookatMat = Matrix4::CreateLookAt(camToWorldMat.GetTranslation(), camToWorldMat.GetTranslation() + newFront, newUp);

		newLookatMat.Invert();
		worldToCamMatrix = newLookatMat;
	}

	void Camera::UpdateCameraTransform(float deltaTime)
	{
		Matrix4 cameraWorldMatrix = worldToCamMatrix;
		cameraWorldMatrix.Invert();

		Vector3 cameraPosition = cameraWorldMatrix.GetTranslation();

		if (movementInput.Length() > 0)
		{
			Vector3 forwardDir = cameraWorldMatrix.GetZAxis();
			Vector3 rightDir = cameraWorldMatrix.GetXAxis();
			Vector3 UpDir = cameraWorldMatrix.GetYAxis();

			Vector3 worldMovement = UpDir * movementInput.y + rightDir * movementInput.x - forwardDir * movementInput.z;
			worldMovement.Normalize();

			cameraPosition += worldMovement * speed * deltaTime;
			// std::cout << "post cam pos: " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
			// std::cout << "Delta time is: " << deltaTime << std::endl;
			cameraWorldMatrix.mat[3][0] = cameraPosition.x;
			cameraWorldMatrix.mat[3][1] = cameraPosition.y;
			cameraWorldMatrix.mat[3][2] = cameraPosition.z;

			worldToCamMatrix = cameraWorldMatrix;
			worldToCamMatrix.Invert();
		}
		cameraData.cameraPosition = cameraPosition;
		cameraData.viewProjection = projectionMatrix * worldToCamMatrix;
		movementInput = Vector3(0, 0, 0);
	}


	void Camera::updateDescriptorSets()
	{
		VulkanDescriptorSet* m_currentDescriptorSet = Renderer::GetCurrentDescriptorSet();
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			m_currentDescriptorSet->UpdateDescriptorSet(cameraBuffer[i], 0, sizeof(CameraBuffer), i);
		}
	}

	void Camera::ProcessInput(double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = (xpos - lastX) * MOUSE_SENSITIVITY;
		float yoffset = (ypos - lastY) * MOUSE_SENSITIVITY;
		lastX = xpos;
		lastY = ypos;

		RotateCamera(xoffset, yoffset);
	}


	void Camera::SetCamData(Vector3 position, Quaternion Rotation)
	{
		Matrix4 rotMat = Matrix4::CreateFromQuaternion(Rotation);
		Matrix4 transMat = Matrix4::CreateTranslation(position);

		worldToCamMatrix = rotMat * transMat;
		worldToCamMatrix.Invert();
	}

	void Camera::UploadCameraBuffer()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			cameraBuffer[i]->UpdateUniformData(&cameraData, sizeof(cameraData));
		}
	}

	void Camera::PrintCurrentCamMatrix()
	{
		std::cout << std::endl;
		std::cout << "My WorldToCam Matrix:" << std::endl;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				std::cout << worldToCamMatrix.mat[col][row] << "\t";  // Compare the actual world-to-cam matrix
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;


		std::cout << std::endl;
		std::cout << "Projection Matrix:" << std::endl;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				std::cout << projectionMatrix.mat[col][row] << "\t";  // Note: [col][row] for GLM
			}
			std::cout << std::endl;
		}
		Vector4 oneVertexCoord = Vector4(-20.f, 0.f, -20.f, 1.f);
		Vector4 oneCustomResult = cameraData.viewProjection * oneVertexCoord;

		std::cout << "my coordinate result is" << oneCustomResult.x << " " << oneCustomResult.y << " " << oneCustomResult.z << " " << oneCustomResult.w << std::endl;
	}
}