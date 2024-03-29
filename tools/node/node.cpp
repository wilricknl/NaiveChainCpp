#include "node.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "blockchain_window.hpp"

// ----------------------------------------------------------------------------
Node::Node(std::string name)
	: Window(std::move(name), true)
{}

// ----------------------------------------------------------------------------
bool Node::Initialize(GLFWwindow* window, const std::string& glVersion)
{
	m_blockchain = std::make_shared<Blockchain>();

	// Create windows
	auto blockchainWindow = std::make_shared<BlockchainWindow>("Blockchain", true, m_blockchain, false);
	m_windows.emplace_back(blockchainWindow);

	// Make pointer to the node available within glfw
	glfwSetWindowUserPointer(window, this);
	
	// Create callbacks
	m_mouseButtonCallback = [this](GLFWwindow* glfwWindow, int button, int action, int mods) -> void
		{
			for (const auto& window : m_windows)
			{
				if (const auto& callback = window->GetMouseButtonCallback())
				{
					callback(glfwWindow, button, action, mods);
				}
			}
		};

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) -> void
		{
			const auto* application = static_cast<Node*>(glfwGetWindowUserPointer(window));
			if (const auto& callback = application->GetMouseButtonCallback())
			{
				callback(window, button, action, mods);
			}
		}
	);

	m_cursorPosCallback = [this](GLFWwindow* glfwWindow, double positionX, double positionY) -> void
		{
			for (const auto& window : m_windows)
			{
				if (const auto& callback = window->GetCursorCallback())
				{
					callback(glfwWindow, positionX, positionY);
				}
			}
		};

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double positionX, double positionY) -> void
		{
			const auto* application = static_cast<Node*>(glfwGetWindowUserPointer(window));
			if (const auto& callback = application->GetCursorCallback())
			{
				callback(window, positionX, positionY);
			}
		}
	);

	m_keyCallback = [this](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) -> void
		{
			for (const auto& window : m_windows)
			{
				if (const auto& callback = window->GetKeyCallback())
				{
					callback(glfwWindow, key, scancode, action, mods);
				}
			}
		};

	glfwSetKeyCallback(window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) -> void
		{
			const auto* application = static_cast<Node*>(glfwGetWindowUserPointer(glfwWindow));
			if (const auto& callback = application->GetKeyCallback())
			{
				callback(glfwWindow, key, scancode, action, mods);
			}
		}
	);

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	return ImGui_ImplGlfw_InitForOpenGL(window, true)
		&& ImGui_ImplOpenGL3_Init(glVersion.c_str());
}

// ----------------------------------------------------------------------------
void Node::Update(GLFWwindow* glfwWindow, double deltaTime)
{
	for (const auto& window : m_windows)
	{
		window->Update(glfwWindow, deltaTime);
	}
}

// ----------------------------------------------------------------------------
void Node::Render()
{
	Begin(ImGuiWindowFlags_None);

	if (m_bShowDemo)
	{
		ImGui::ShowDemoWindow(&m_bShowDemo);
	}

	for (const auto& window : m_windows)
	{
		window->Render();
	}

	End();
}

// ----------------------------------------------------------------------------
bool Node::Quit() const
{
	return m_bQuit;
}

// ----------------------------------------------------------------------------
void Node::Destroy() const
{
	// Destroy ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

// ----------------------------------------------------------------------------
bool Node::Begin(ImGuiWindowFlags flags)
{
	// Start ImGui render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Setup docking
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
	flags =
		ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavFocus
		| ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	auto bResult = Window::Begin(flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGuiID dockspaceId = ImGui::GetID("dockspace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

	static bool bFirstTime = true;
	if (bFirstTime)
	{
		bFirstTime = false;

		ImGui::DockBuilderRemoveNode(dockspaceId); // Clear any previous layout
		ImGui::DockBuilderAddNode(dockspaceId, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspaceId, viewport->Size);

		ImGuiID dockBlockchainWindow;
		ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.0f, nullptr, &dockBlockchainWindow);

		ImGui::DockBuilderDockWindow("Blockchain", dockBlockchainWindow);
		ImGui::DockBuilderFinish(dockspaceId);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Demo", nullptr, &m_bShowDemo);
			ImGui::Separator();
			ImGui::MenuItem("Quit", "Alt+F4", &m_bQuit);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			for (const auto& window : m_windows)
			{
				if (window->IsCanClose())
				{
					ImGui::MenuItem(window->GetName().c_str(), nullptr, window->Show());
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	return bResult;
}

// ----------------------------------------------------------------------------
void Node::End()
{
	Window::End();

	// Finish ImGui render
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
