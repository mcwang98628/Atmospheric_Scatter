#include "stdafx.h"
#include "camera.h"
#include "EventManager.h"
#include "Render/Vulkan/vulkanControl.h"

namespace StudyEngine {

	Camera::Camera()
	{

	}

	Camera::~Camera()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyBuffer(VulkanControl::Get()->GetDeviceContext(), cameraBuffer[i], nullptr);
			vkFreeMemory(VulkanControl::Get()->GetDeviceContext(), cameraBufferMemory[i], nullptr);
		}
	}

	void Camera::Init()
	{
		VkDeviceSize cameraSize = sizeof(CameraBuffer);

		cameraBuffer.resize(MAX_FRAMES_IN_FLIGHT);
		cameraBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
		cameraBufferMapped.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			VulkanControl::Get()->createBuffer(cameraSize,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				cameraBuffer[i],
				cameraBufferMemory[i]);
			vkMapMemory(VulkanControl::Get()->GetDeviceContext(), cameraBufferMemory[i], 0, cameraSize, 0, &cameraBufferMapped[i]);
		}
		updateDescriptorSets();
		worldToCamMatrix = Matrix4::CreateTranslation(Vector3(0.0f, 1.0f, 0.0f)) * Matrix4::CreateRotationY(Math::Pi / 4);
		yaw = Math::ToDegrees(Math::Pi / 4);

		worldToCamMatrix.Invert();
		projectionMatrix = Matrix4::CreatePerspectiveFOV(fov, float(WIDTH), (float)HEIGHT, near, far);
	}

	void Camera::moveForward(float velocity) {
		Matrix4 cameraWorldMatrix = worldToCamMatrix;
		cameraWorldMatrix.Invert();

		Vector3 forwardDir = cameraWorldMatrix.GetZAxis();

		Vector3 cameraPosition = cameraWorldMatrix.GetTranslation();
		cameraPosition -= velocity * forwardDir;

		cameraWorldMatrix.mat[3][0] = cameraPosition.x;
		cameraWorldMatrix.mat[3][1] = cameraPosition.y;
		cameraWorldMatrix.mat[3][2] = cameraPosition.z;

		worldToCamMatrix = cameraWorldMatrix;
		worldToCamMatrix.Invert();
	}

	void Camera::moveHorizontal(float velocity) {
		Matrix4 cameraWorldMatrix = worldToCamMatrix;
		cameraWorldMatrix.Invert();

		Vector3 rightDir = cameraWorldMatrix.GetXAxis();

		Vector3 cameraPosition = cameraWorldMatrix.GetTranslation();
		cameraPosition += velocity * rightDir;

		cameraWorldMatrix.mat[3][0] = cameraPosition.x;
		cameraWorldMatrix.mat[3][1] = cameraPosition.y;
		cameraWorldMatrix.mat[3][2] = cameraPosition.z;

		worldToCamMatrix = cameraWorldMatrix;
		worldToCamMatrix.Invert();
	}

	void Camera::moveVertical(float velocity) {
		Matrix4 cameraWorldMatrix = worldToCamMatrix;
		cameraWorldMatrix.Invert();

		Vector3 forwardDir = cameraWorldMatrix.GetYAxis();

		Vector3 cameraPosition = cameraWorldMatrix.GetTranslation();
		cameraPosition += velocity * forwardDir;

		cameraWorldMatrix.mat[3][0] = cameraPosition.x;
		cameraWorldMatrix.mat[3][1] = cameraPosition.y;
		cameraWorldMatrix.mat[3][2] = cameraPosition.z;

		worldToCamMatrix = cameraWorldMatrix;
		worldToCamMatrix.Invert();
	}

	void Camera::UpdateCameraTransform(float xoffset, float yoffset)
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
		/*std::cout << "new Yaw" << yaw << std::endl;
		std::cout << "new pitch" << pitch << std::endl;*/
		Vector3 newFront;
		newFront.x = -sinf(Math::ToRadians(yaw)) * cosf(Math::ToRadians(pitch));
		newFront.y = -sinf(Math::ToRadians(pitch));
		newFront.z = cosf(Math::ToRadians(yaw)) * cosf(Math::ToRadians(pitch));
		newFront.Normalize();
		Matrix4 camToWorldMat = worldToCamMatrix;
		camToWorldMat.Invert();
		/*std::cout << "My camToWorldMat Before:" << std::endl;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				std::cout << camToWorldMat.mat[col][row] << "\t";
			}
			std::cout << std::endl;
		}*/
		Vector3 worldUp = Vector3(0.f, 1.f, 0.f);
		Vector3 newRight = Vector3::Cross(newFront, worldUp);
		Vector3 newUp = Vector3::Cross(newRight, newFront);

		//Vector3 curTranslation = camToWorldMat.GetTranslation();
		//std::cout << "My cam Translation before is:" << std::endl;
		//std::cout << curTranslation.x << "\t" << curTranslation.y << "\t" << curTranslation.z << "\t";
		//std::cout << std::endl;
		Matrix4 newLookatMat = Matrix4::CreateLookAt(camToWorldMat.GetTranslation(), camToWorldMat.GetTranslation() + newFront, newUp);
		/*std::cout << "My camToWorldMat After:" << std::endl;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				std::cout << newLookatMat.mat[col][row] << "\t";
			}
			std::cout << std::endl;
		}*/
		newLookatMat.Invert();
		worldToCamMatrix = newLookatMat;

	}

	void Camera::updateCameraBuffer(uint32_t currentImage)
	{
		Matrix4 worldToCamMatrixInv = worldToCamMatrix;
		worldToCamMatrixInv.Invert();
		cameraData.cameraPosition = worldToCamMatrixInv.GetTranslation();

		cameraData.viewProjection = projectionMatrix * worldToCamMatrix;

		// --- DEBUG PRINT ---
	// This will print the matrices and a test coordinate calculation ONCE.
		static bool hasPrinted = false;
		/*if (!hasPrinted)
		{
			std::cout << std::endl << "--- DEBUG ---" << std::endl;

			std::cout << "Camera World Position: "
				<< cameraData.cameraPosition.x << ", "
				<< cameraData.cameraPosition.y << ", "
				<< cameraData.cameraPosition.z << std::endl << std::endl;

			std::cout << "My View Matrix (worldToCamMatrix):" << std::endl;
			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					std::cout << worldToCamMatrix.mat[col][row] << "\t";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;

			std::cout << "My Projection Matrix:" << std::endl;
			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					std::cout << projectionMatrix.mat[col][row] << "\t";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;

			Vector4 oneVertexCoord = Vector4(-20.f, 0.f, -20.f, 1.f);
			Vector4 oneCustomResult = cameraData.viewProjection * oneVertexCoord;

			std::cout << "my coordinate result is: "
				<< oneCustomResult.x << " " << oneCustomResult.y << " "
				<< oneCustomResult.z << " " << oneCustomResult.w << std::endl;

			std::cout << "--- END DEBUG ---" << std::endl << std::endl;
			hasPrinted = true;
		}*/
		// --- END DEBUG PRINT ---
		memcpy(cameraBufferMapped[currentImage], &cameraData, sizeof(cameraData));
	}


	void Camera::updateDescriptorSets()
	{

		std::vector<VkWriteDescriptorSet> descriptorWrites(2);
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			VkDescriptorSet descriptorSet = VulkanControl::Get()->getDescriptorSet(i);

			VkDescriptorBufferInfo camInfo{};
			camInfo.buffer = cameraBuffer[i];
			camInfo.offset = 0;
			camInfo.range = sizeof(CameraBuffer);

			descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[i].dstSet = descriptorSet;
			descriptorWrites[i].dstBinding = 0;
			descriptorWrites[i].dstArrayElement = 0;
			descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[i].descriptorCount = 1;
			descriptorWrites[i].pBufferInfo = &camInfo;

			vkUpdateDescriptorSets(VulkanControl::Get()->GetDeviceContext(), 1, &descriptorWrites[i], 0, nullptr);
		}
	}

	//VkDescriptorSetLayoutBinding Camera::getCameraDescriptorLayoutBinding()
	//{
	//	VkDescriptorSetLayoutBinding cameraBinding{};
	//	cameraBinding.binding            = 0;
	//	cameraBinding.descriptorCount    = 1;
	//	cameraBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//	cameraBinding.pImmutableSamplers = nullptr;
	//	cameraBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	//	
	//	return cameraBinding;
	//}

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

		UpdateCameraTransform(xoffset, yoffset);
	}


	void Camera::SetCamData(Vector3 position, Quaternion Rotation)
	{
		Matrix4 rotMat = Matrix4::CreateFromQuaternion(Rotation);
		Matrix4 transMat = Matrix4::CreateTranslation(position);

		worldToCamMatrix = rotMat * transMat;
		worldToCamMatrix.Invert();
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