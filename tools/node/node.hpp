#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <list>
#include <memory>
#include <string>

#include "blockchain.hpp"
#include "window.hpp"

// ----------------------------------------------------------------------------
/**
 * NaiveChain node
 */
class Node final : public Window
{
public:
	/**
	 * Constructor
	 * @param name The application name
	 */
	Node(std::string name);

	/**
	 * Initialize application with glfw context
	 * @param window The glfw window
	 * @param glVersion The OpenGL version to use
	 */
	bool Initialize(GLFWwindow* window, const std::string& glVersion = "#version 450");

	/**
	 * Next program step
	 * @param glfwWindow The glfw window
	 * @param deltaTime Elapsed time since previous step
	 */
	void Update(GLFWwindow* glfwWindow, double deltaTime) override;

	/**
	 * Render the application
	 */
	void Render() override;

	/**
	 * Should application quit
	 */
	bool Quit() const;

	/**
	 * Destroy application resources
	 */
	void Destroy() const;

protected:
	/**
	 * Begin render
	 * @param flags The window flags
	 */
	bool Begin(ImGuiWindowFlags flags) override;

	/**
	 * End render
	 */
	void End() override;

private:
	/// Blockchain
	std::shared_ptr<Blockchain> m_blockchain;

	/// All sub-windows
	std::list<std::shared_ptr<Window>> m_windows;

	/// Check if application is finished
	bool m_bQuit = false;

	/// Should show demo?
	bool m_bShowDemo = false;
};
