#ifndef _D3D11_Render_Window_H_
#define _D3D11_Render_Window_H_
#pragma once

namespace Air
{
	class D3D11RenderWindow
	{
	public:
		D3D11RenderWindow();
		~D3D11RenderWindow();

		bool isFullScreen() const;

		void setFullScreen(bool fs);
	};


}

#endif