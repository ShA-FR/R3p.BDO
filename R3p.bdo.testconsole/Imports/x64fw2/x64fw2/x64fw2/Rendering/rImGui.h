#pragma once

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../Utilities/WindowHandle.h"
#include "../ImGui/imgui_internal.h"
#include <sstream>
#include "../Types/Vector2.h"
#include "../Types/Vector3.h"
#include <iomanip>
#include <vector>

namespace Rendering
	{		
		bool _Init = false;

		bool _SomeWindow0 = false;
		bool _SomeWindow1 = false;

		static bool InitIMGui(ID3D11Device *pDevice, ID3D11DeviceContext *pContext)
		{			
			_Init = ImGui_ImplDX11_Init(GetDynamicWindowHandle(), pDevice, pContext);

			return _Init;
		}
				
		static void DrawMainPanel()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("R3p.BDO"))
				{
					if (ImGui::MenuItem("SomeWindow0"))
					{
						if (_SomeWindow0)
							_SomeWindow0 = false;
						else
							_SomeWindow0 = true;
					}

					if (ImGui::MenuItem("SomeWindow1"))
					{
						if (_SomeWindow1)
							_SomeWindow1 = false;
						else
							_SomeWindow1 = true;
					}
					
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}

			if (_SomeWindow0)
			{
				if (ImGui::Begin("SomeWindow0", &_SomeWindow0))
				{
					if (ImGui::Button("Some Button"))
					{
						
					}										

					ImGui::End();
				}
			}

			if (_SomeWindow1)
			{
				if (ImGui::Begin("SomeWindow1", &_SomeWindow1))
				{
					if (ImGui::Button("Some Button"))
					{

					}

					ImGui::End();
				}
			}
		}
		
		inline void BeginScene()
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::Begin("BackBuffer", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

			ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
			ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);
		}

		inline void EndScene()
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			window->DrawList->PushClipRectFullScreen();

			ImGui::End();
			ImGui::PopStyleColor();
		}

		inline float rDrawText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			float a = (color >> 24) & 0xff;
			float r = (color >> 16) & 0xff;
			float g = (color >> 8) & 0xff;
			float b = (color) & 0xff;

			std::stringstream steam(text);
			std::string line;

			float y = 0.0f;
			int i = 0;

			while (std::getline(steam, line))
			{
				ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

				if (center)
				{
					window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
					window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
					window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
					window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());

					window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
				}
				else
				{
					window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
					window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
					window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
					window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());

					window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
				}

				y = pos.y + textSize.y * (i + 1);
				i++;
			}

			return y;
		}

		inline void rDrawWorldLine(Vector2 from, Vector2 to, uint32_t color, float thickness)
		{			
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			float a = (color >> 24) & 0xff;
			float r = (color >> 16) & 0xff;
			float g = (color >> 8) & 0xff;
			float b = (color) & 0xff;

			window->DrawList->AddLine(ImVec2(from.X, from.Y), ImVec2(to.X, to.Y), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), thickness);
		}

		inline void rDrawLine(Vector2 from, Vector2 to, uint32_t color, float thickness)
		{			
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			float a = (color >> 24) & 0xff;
			float r = (color >> 16) & 0xff;
			float g = (color >> 8) & 0xff;
			float b = (color) & 0xff;

			window->DrawList->AddLine(ImVec2(from.X, from.Y), ImVec2(to.X, to.Y), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), thickness);
		}

		inline void rDrawWorldCircle(std::vector<Vector2> pts, float radius, uint32_t color, float thickness, bool filled = false)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			float a = (color >> 24) & 0xff;
			float r = (color >> 16) & 0xff;
			float g = (color >> 8) & 0xff;
			float b = (color) & 0xff;
			
			ImVec2* _pts = new ImVec2[pts.size()];

			for (int i = 0; i < pts.size(); i++)
			{
				_pts[i] = ImVec2(pts[i].X, pts[i].Y);
			}

			if(!filled)
				window->DrawList->AddPolyline(_pts, pts.size(), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), true, thickness, false);
			else
				window->DrawList->AddConvexPolyFilled(_pts, pts.size(), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), false);
		}

		inline void rDrawCircle(Vector2 position, float radius, uint32_t color, float thickness, bool filled = false)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			float a = (color >> 24) & 0xff;
			float r = (color >> 16) & 0xff;
			float g = (color >> 8) & 0xff;
			float b = (color) & 0xff;
			
			if(!filled)
				window->DrawList->AddCircle(ImVec2(position.X, position.Y), radius, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 12, thickness);
			else
				window->DrawList->AddCircleFilled(ImVec2(position.X, position.Y), radius, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 12);
		}
				
		inline void rDrawWorldBox(std::vector<Vector2> pts, float size, uint32_t color, float thickness, float height = -1)
		{									
			rDrawLine(pts[0], pts[1], color, thickness);
			rDrawLine(pts[1], pts[2], color, thickness);
			rDrawLine(pts[2], pts[3], color, thickness);
			rDrawLine(pts[3], pts[0], color, thickness);

			rDrawLine(pts[4], pts[5], color, thickness);
			rDrawLine(pts[5], pts[6], color, thickness);
			rDrawLine(pts[6], pts[7], color, thickness);
			rDrawLine(pts[7], pts[4], color, thickness);

			rDrawLine(pts[4], pts[0], color, thickness);
			rDrawLine(pts[5], pts[1], color, thickness);
			rDrawLine(pts[6], pts[2], color, thickness);
			rDrawLine(pts[7], pts[3], color, thickness);
		}

		inline void rDrawWorldSquare(std::vector<Vector2> pts, float size, uint32_t color, float thickness, bool filled = false)
		{			
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			float a = (color >> 24) & 0xff;
			float r = (color >> 16) & 0xff;
			float g = (color >> 8) & 0xff;
			float b = (color) & 0xff;

			ImVec2* _pts = new ImVec2[pts.size()];

			for (int i = 0; i < pts.size(); i++)
			{
				_pts[i] = ImVec2(pts[i].X, pts[i].Y);
			}
			
			if (!filled)
				window->DrawList->AddPolyline(_pts, pts.size(), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), true, thickness, false);
			else
				window->DrawList->AddConvexPolyFilled(_pts, pts.size(), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), false);
		}

		inline void rDrawSquare(Vector2 center, float size, uint32_t color, float thickness, bool filled = false)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			float a = (color >> 24) & 0xff;
			float r = (color >> 16) & 0xff;
			float g = (color >> 8) & 0xff;
			float b = (color) & 0xff;

			ImVec2* pts = new ImVec2[4];
						
			pts[0] = ImVec2(center.X - (size/2), center.Y + (size/2));
			pts[1] = ImVec2(center.X - (size / 2), center.Y - (size / 2));
			pts[2] = ImVec2(center.X + (size / 2), center.Y - (size / 2));
			pts[3] = ImVec2(center.X + (size / 2), center.Y + (size / 2));

			if (!filled)
				window->DrawList->AddPolyline(pts, 4, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), true, thickness, false);
			else
				window->DrawList->AddConvexPolyFilled(pts, 4, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), false);
		}
				
		inline bool OnWindowMessage(const MSG* lpMsg)
		{			
			if (_Init && lpMsg)
			{								
				ImGuiIO& io = ImGui::GetIO();
				if (GetDynamicWindowHandle() == lpMsg->hwnd)
				{
					switch (lpMsg->message)
					{
					case WM_LBUTTONDOWN:
						return io.WantCaptureMouse;
					case WM_LBUTTONUP:
						return io.WantCaptureMouse;
					case WM_LBUTTONDBLCLK:
						return io.WantCaptureMouse;
					case WM_RBUTTONDOWN:
						return io.WantCaptureMouse;
					case WM_RBUTTONUP:
						return io.WantCaptureMouse;
					case WM_RBUTTONDBLCLK:
						return io.WantCaptureMouse;
					case WM_MBUTTONDOWN:
						return io.WantCaptureMouse;
					case WM_MBUTTONUP:
						return io.WantCaptureMouse;
					case WM_MBUTTONDBLCLK:
						return io.WantCaptureMouse;
					case WM_MOUSEWHEEL:
						io.MouseWheel += GET_WHEEL_DELTA_WPARAM(lpMsg->wParam) > 0 ? +1.0f : -1.0f;
						return io.WantCaptureMouse;
					case WM_MOUSEMOVE:
						return io.WantCaptureMouse;
					case WM_KEYDOWN:
						if (lpMsg->wParam < 256)
							io.KeysDown[lpMsg->wParam] = 1;
						return io.WantCaptureKeyboard;
					case WM_KEYUP:
						if (lpMsg->wParam < 256)
							io.KeysDown[lpMsg->wParam] = 0;
						return io.WantCaptureKeyboard;
					case WM_CHAR:
						if (lpMsg->wParam > 0 && lpMsg->wParam < 0x10000)
							io.AddInputCharacter((unsigned short)lpMsg->wParam);
						return io.WantCaptureKeyboard || io.WantTextInput;
					}
				}
			}

			return false;
		}

		static void Draw()
		{	
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(GetDynamicWindowHandle(), &p);

			ImGuiIO& imgIO = ImGui::GetIO();				
			imgIO.MousePos.x = p.x;
			imgIO.MousePos.y = p.y;
			imgIO.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON);
			imgIO.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON);
			imgIO.MouseDown[2] = GetAsyncKeyState(VK_MBUTTON);			
			imgIO.KeyCtrl = GetAsyncKeyState(VK_CONTROL);
			imgIO.KeyAlt = GetAsyncKeyState(VK_MENU);
			imgIO.KeyShift = GetAsyncKeyState(VK_SHIFT);

			ImGui_ImplDX11_NewFrame();

			DrawMainPanel();
									
			ImGui::Render();
		}
	}
