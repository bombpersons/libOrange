#ifndef ORANGE_NOCOPY
#define ORANGE_NOCOPY

namespace orange {

	/// Inherit from this class if you want the compiler to prevent you from copy any instance of that class.
	/// Pretty much the same as sf::NonCopyable from SFML.
	class NoCopy {
	protected:
		NoCopy() {}
	private:
		NoCopy(const NoCopy&);
		NoCopy& operator =(const NoCopy&);
	};
}

#endif