#ifndef ORANGE_GLCONTEXT
#define ORANGE_GLCONTEXT

// This file is heavily based on the SMFL GlContext class.
#include <Orange/util/NoCopy.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

namespace orange {
	namespace priv {
		class WindowImpl;
	}
	class GLContext : NoCopy {
	public:
		struct Settings {
			Settings(unsigned int _color = 24,
							 unsigned int _alpha = 8,
							 unsigned int _depth = 24,
							 unsigned int _stencil = 8,
							 unsigned int _antialiasing = 0,
						   unsigned int _major = 4,
							 unsigned int _minor = 0) {
					ColorBits = _color;
					AlphaBits = _alpha;
					DepthBits = _depth;
					StencilBits = _stencil;
					AntialiasingLevel = _antialiasing;
					MajorVersion = _major;
					MinorVersion = _minor;
			}
			unsigned int ColorBits;
			unsigned int AlphaBits;
			unsigned int DepthBits;
			unsigned int StencilBits;
			unsigned int AntialiasingLevel;
			unsigned int MajorVersion;
			unsigned int MinorVersion;
		};
	public:
		GLContext();
		virtual ~GLContext();

		// Init
		void Init();

		// Set a the context active
		bool SetActive(bool _v);

		// Get the settings
		const Settings& GetSettings();

		// Display / Swapbuffers / Present / whatever you want to call it.
		virtual void Display() = 0;

		// Enable / Disable vertical sync
		virtual void SetVSync(bool _v) = 0;

		// Static functions
		static void StaticInit();
		static void StaticShutdown();

		// Ensures that the current thread has a
		// current opengl context. It will create one
		// if one doesn't exist.
		static void EnsureContext();

		// Create a new context
		static GLContext* Create();
		static GLContext* Create(const GLContext::Settings& _settings, const priv::WindowImpl* _window);
		static GLContext* Create(const GLContext::Settings& _settings, unsigned int _width, unsigned int _height);
	protected:
		// Make this context current
		virtual bool MakeCurrent() = 0;

		// Context settings.
		GLContext::Settings settings;
	private:
	};
}

#endif
