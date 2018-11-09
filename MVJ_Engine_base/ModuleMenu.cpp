#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleMenu.h"
#include "ModuleRender.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <stdio.h>
#include <SDL.h>
#include "ModuleRenderExercise.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"

ModuleMenu::ModuleMenu()
{
}


ModuleMenu::~ModuleMenu()
{
}

bool ModuleMenu::Init() {
	// Setup window
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	//setting up the vars for the logs
	logMSIterator = 0;
	logFPSIterator = 0;
	fps_log = new float[50];
	ms_log = new float[50];
	lastFrameTime = SDL_GetTicks();
	lastSecondTime = SDL_GetTicks();
	
	showWindows = true;
	ImGui::StyleColorsDark();

	ImVec4 olive = { 0.6f,0.6f,0.0f,1.0f };
	ImVec4 oliveLighter = { 0.7f,0.7f,0.1f,1.0f };
	ImVec4 oliveLTrans = { 0.5f,0.5f,0.f,.7f };


	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = { 0.1f,0.1f,0.1f,0.7f };
	//ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] = { 0.05f, 0.05f,0.0f,1.0f };
	//ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = { 0.05f, 0.05f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = { 0.2f, 0.2f,0.1f,1.0f };

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Header] = olive;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_HeaderHovered] = oliveLighter;

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Button] = olive;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_ButtonHovered] = oliveLighter;

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBg] = oliveLTrans;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgActive] = oliveLTrans;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = oliveLTrans;
	/*
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Tab] = { 0.3f,0.7f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TabHovered] = { 0.8f,0.35f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TabUnfocused] = { 0.5f,0.2f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TabUnfocusedActive] = { 0.65f,0.25f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TabActive] = { 0.9f,0.45f,0.0f,1.0f };

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Header] = { 0.8f,0.37f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_HeaderActive] = { 1.0f,0.6f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_HeaderHovered] = { 0.95f,0.5f,0.0f,1.0f };

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Button] = { 0.9f,0.45f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_ButtonActive] = { 0.8f,0.37f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_ButtonHovered] = { 0.95f,0.5f,0.0f,1.0f };

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBg] = { 1.0f,0.6f,0.0f,0.2f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgActive] = { 0.8f,0.37f,0.0f,0.5f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = { 0.95f,0.5f,0.0f,0.5f };
	*/
	return true;
}

void ModuleMenu::updateFramerates() {
	double timeElapsed = SDL_GetTicks() - lastSecondTime;
	//fps calculation
	lastSecondTime = SDL_GetTicks();
	fps_log[logFPSIterator] = 1000/timeElapsed;
	++logFPSIterator;
	if (logFPSIterator > 49) logFPSIterator = 0;
	//ms calculation
	ms_log[logMSIterator] = timeElapsed;
	lastFrameTime = SDL_GetTicks();
	//iterator increase
	++logMSIterator;
	if (logMSIterator > 49) logMSIterator = 0;
}

update_status ModuleMenu::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::Update() {
	
	int consoleHeight = App->camera->screenHeight * 1.f/4;
	ImVec2 mainMenuSize;
	// Menu superior
	if (ImGui::BeginMainMenuBar())
	{
		mainMenuSize = ImGui::GetWindowSize();
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::CollapsingHeader("Window options"))
			{
				ImGui::Checkbox("Show windows", &showWindows); 
			}
			
			if (ImGui::Button("Go to GitHub")) {
				ShellExecute(NULL, "open", "https://github.com/expelthegrace/Engine_1st_Assign", NULL, NULL, SW_SHOWNORMAL);
			}

			ImGui::EndMenu();
		}
		if (ImGui::Button("Quit")) {
			return UPDATE_STOP;
		}

		ImGui::EndMainMenuBar();
		/*
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::Button("Documentation")) {
				ShellExecute(NULL, "open", "https://github.com/RogerNogue/MVJ_Engine_base/wiki", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::Button("Download latest")) {
				ShellExecute(NULL, "open", "https://github.com/RogerNogue/MVJ_Engine_base/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::Button("Report a bug")) {
				ShellExecute(NULL, "open", "https://github.com/RogerNogue/MVJ_Engine_base/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}
		*/		
	}

	if (showWindows) {
		ImGui::SetNextWindowPos(ImVec2(App->camera->screenWidth / 4, App->camera->screenHeight - consoleHeight), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(App->camera->screenWidth / 2, consoleHeight), ImGuiCond_FirstUseEver);

		console.Draw("Console");

		ImGui::SetNextWindowPos(ImVec2(0, mainMenuSize.y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(App->camera->screenWidth / 4, (App->camera->screenHeight - mainMenuSize.y)/2), ImGuiCond_FirstUseEver);
		bool obert = true;
		ImGui::Begin("Properties", &obert);
			
		if (ImGui::CollapsingHeader("Transformation"))
		{
			float v3pos [3] = { App->modelLoader->modelPosition.x,App->modelLoader->modelPosition.y,App->modelLoader->modelPosition.z };
			ImGui::InputFloat3("Position", v3pos);
			float v3rot[3] = { App->modelLoader->modelRotation.x,App->modelLoader->modelRotation.y,App->modelLoader->modelRotation.z };
			ImGui::InputFloat3("Rotation", v3rot);
			float v3scale[3] = { App->modelLoader->modelScale.x,App->modelLoader->modelScale.y,App->modelLoader->modelScale.z };
			ImGui::InputFloat3("Scale", v3scale);
						
		}
		if (ImGui::CollapsingHeader("Geometry"))
		{
			ImGui::Text("Number of triangles: %i \n",App->modelLoader->numFaces);
		}
		if (ImGui::CollapsingHeader("Texture"))
		{
			ImGui::Checkbox("Draw texture", &App->renderer->renderTexture);
		}

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, (App->camera->screenHeight - mainMenuSize.y) / 2), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(App->camera->screenWidth / 4, (App->camera->screenHeight - mainMenuSize.y) / 2 + mainMenuSize.y), ImGuiCond_FirstUseEver);
		ImGui::Begin("Configuration", &obert);

		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse position: %i , %i", App->input->mouse_position.x, App->input->mouse_position.y);
			ImGui::Text("Mousewheel force: %i", App->input->mouseWheel);
			
		}
		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::BulletText("Camera position: ( %f, %f, %f )", App->camera->camPos.x, App->camera->camPos.y, App->camera->camPos.z);
			ImGui::BulletText("Up directions: ( %f, %f, %f )", App->camera->up.x, App->camera->up.y, App->camera->up.z);
			ImGui::BulletText("Forward directions: ( %f, %f, %f )", App->camera->fwd.x, App->camera->fwd.y, App->camera->fwd.z);
			ImGui::BulletText("Side directions: ( %f, %f, %f )", App->camera->side.x, App->camera->side.y, App->camera->side.z);
			ImGui::NewLine();
			ImGui::InputFloat("Near Plane", &App->camera->frustum.nearPlaneDistance);
			ImGui::InputFloat("Far Plane", &App->camera->frustum.farPlaneDistance);
			ImGui::NewLine();
			ImGui::Checkbox("Show grid", &App->renderer->showGrid);
			
		
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			int lastW = App->camera->screenWidth;
			int lastH = App->camera->screenHeight;
			int actualW = App->camera->screenWidth;
			int actualH = App->camera->screenHeight;


			ImGui::InputInt("Window width", &actualW,20,200);
			ImGui::InputInt("Window height", &actualH,20,200);

			if (actualW != lastW || actualH != lastH) {
				App->renderer->WindowResized(actualW, actualH);
				SDL_SetWindowSize(App->window->window, actualW, actualH);
			}			

		}
		if (ImGui::CollapsingHeader("Stats"))
		{
			ImGui::Text("Application Time = %d", SDL_GetTicks() / 1000);
			char* title = new char[50];
			updateFramerates();
			sprintf_s(title, 50, "Framerate %.1f", fps_log[logFPSIterator]);
			ImGui::PlotHistogram("", fps_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));
			sprintf_s(title, 50, "Milliseconds %.1f", ms_log[logMSIterator]);
			ImGui::PlotHistogram("", ms_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));
		}
	
		ImGui::End();
	}

	/*
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("About"))
	{
		ImGui::BulletText("Engine name: DESPACITO 2");
		ImGui::Text("Description");
		ImGui::BulletText("DESPACITO 2 allows you to create AAA-quality games with little to none effort.");
		ImGui::Text("Authors");
		ImGui::BulletText("Roger Nogue Ballbe.");
		ImGui::Text("Libraries");
		ImGui::BulletText("SDL.");
		ImGui::BulletText("IMGUI.");
		ImGui::BulletText("GLEW.");
		ImGui::Text("Licenses");
		if (ImGui::Button("SDL")) {
			ShellExecute(NULL, "open", "https://www.libsdl.org/license.php", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::Button("IMGUI")) {
			ShellExecute(NULL, "open", "https://github.com/ocornut/imgui/blob/master/LICENSE.txt", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::Button("GLEW")) {
			ShellExecute(NULL, "open", "https://www.opengl.org/about/#11", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::Separator();
	}
		
	//going over all the menus
	//App info
	if (ImGui::CollapsingHeader("Configuration"))
	{
		ImGui::Text("Application Time = %d", SDL_GetTicks()/1000);
		char* title = new char[50];
		updateFramerates();
		sprintf_s(title, 50, "Framerate %.1f", fps_log[logFPSIterator]);
		ImGui::PlotHistogram("", fps_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));
		sprintf_s(title, 50, "Milliseconds %.1f", ms_log[logMSIterator]);
		ImGui::PlotHistogram("", ms_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));
	}
	//global variables
	if (ImGui::CollapsingHeader("Globals"))
	{
		ImGui::Text("Screen Width = %d", SCREEN_WIDTH);
		ImGui::Text("Screen Height = %d", SCREEN_HEIGHT);
		ImGui::Text("Fullscreen = %d", FULLSCREEN);
		ImGui::Text("VSYNC = %d", VSYNC);
		ImGui::Text("GLSL version = %s", GLSL_VERSION);
	}
	//module window
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Text("Not much to be shown about this module yet.");
	}
	//module menu
	if (ImGui::CollapsingHeader("Menu"))
	{
		ImGui::Text("Not much to be shown about this module yet.");
	}
	//module render
	if (ImGui::CollapsingHeader("Render"))
	{
		ImGui::Text("Not much to be shown about this module yet.");
	}
	//module textures
	if (ImGui::CollapsingHeader("Textures"))
	{
		ImGui::Text("Not much to be shown about this module yet.");
	}
	//module input
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Not much to be shown about this module yet.");
	}
	//module render exercise
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Cam declarations:");
		ImGui::BulletText("Camera position = ( %f, %f, %f )", App->camera->cam.x, App->camera->cam.y, App->camera->cam.z);
		ImGui::BulletText("VRP position = ( %f, %f, %f )", App->camera->vrp.x, App->camera->vrp.y, App->camera->vrp.z);
		ImGui::BulletText("Up directions = ( %f, %f, %f )", App->camera->up.x, App->camera->up.y, App->camera->up.z);
		ImGui::BulletText("Forward directions= ( %f, %f, %f )", App->camera->fwd.x, App->camera->fwd.y, App->camera->fwd.z);
		ImGui::BulletText("Side directions = ( %f, %f, %f )", App->camera->side.x, App->camera->side.y, App->camera->side.z);
		ImGui::BulletText("Distance between camera and VRP = %f", App->camera->distCamVrp);
	}
	//module program
	if (ImGui::CollapsingHeader("Program"))
	{
		ImGui::Text("Not much to be shown about this module yet.");
	}
	*/
	
	

	//ImGui::End();
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::PostUpdate() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

bool ModuleMenu::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	delete fps_log;
	delete ms_log;
	return true;
}

