#include "blockchain_window.hpp"

#include <array>

// ----------------------------------------------------------------------------
BlockchainWindow::BlockchainWindow(
	std::string name,
	bool bShow,
	std::shared_ptr<Blockchain> blockchain,
	bool bCanClose)
	: Window(std::move(name), bShow, bCanClose)
	, m_blockchain{ std::move(blockchain) }
{}

// ----------------------------------------------------------------------------
void BlockchainWindow::Update(GLFWwindow* glfwWindow, double deltaTime)
{}

// ----------------------------------------------------------------------------
void BlockchainWindow::Render()
{
	if (!m_bShow || !Begin(ImGuiWindowFlags_None))
	{
		return;
	}

    // Visualize blockchain in a table
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
    if (ImGui::BeginTable("blockchain table", 5, flags))
    {
        ImGui::TableSetupColumn("Index");
        ImGui::TableSetupColumn("Timestamp");
        ImGui::TableSetupColumn("Data");
        ImGui::TableSetupColumn("Previous hash");
        ImGui::TableSetupColumn("Hash");
        ImGui::TableHeadersRow();

        for (const auto& block : m_blockchain->GetBlocks())
        {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(std::to_string(block.index).c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(std::to_string(block.timestamp).c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(block.data.c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(block.previous_hash.c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(block.hash.c_str());
        }

        ImGui::EndTable();
    }

    // Add blocks via input field, limit block data to 1024 characters
    static std::array<char, 1024> block_data{};
    ImGui::InputText("Blockdata", block_data.data(), block_data.size());
    ImGui::SameLine();
    if (ImGui::Button("Add block") && block_data.at(0) != '\0')
    {
        // Create block
        m_blockchain->CreateBlock(std::string{ block_data.data() });

        // Clear input field
        block_data.fill('\0');
    }

	End();
}

// ----------------------------------------------------------------------------
bool BlockchainWindow::Begin(ImGuiWindowFlags flags)
{
	return Window::Begin(flags);
}

// ----------------------------------------------------------------------------
void BlockchainWindow::End()
{
	Window::End();
}
