#pragma once

#include <memory>

#include "blockchain.hpp"
#include "window.hpp"

// ----------------------------------------------------------------------------
/**
 * Blockchain window
 */
class BlockchainWindow final : public Window
{
public:
    /**
     * Constructor
     * @param name The name of the window
     * @param bShow Should show the window
     * @param blockchain The blockchain to display
     * @param bCanClose Can close the window
     */
    explicit BlockchainWindow(
        std::string name,
        bool bShow,
        std::shared_ptr<Blockchain> blockchain,
        bool bCanClose = true);

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
    /// Blockchain to interact with
    std::shared_ptr<Blockchain> m_blockchain;
};
