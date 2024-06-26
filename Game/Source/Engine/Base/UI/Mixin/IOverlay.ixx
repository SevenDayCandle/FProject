export module fbc.IOverlay;

namespace fbc {
	export struct IOverlay {
	public:
		virtual ~IOverlay() = default;

		inline virtual void dispose() {};
		inline virtual void open() {}
		inline virtual void refreshDimensions() {}  // Force resizing of hitboxes when the screen size changes

		virtual bool tickUpdate();

		virtual void render() = 0;
		virtual void update() = 0;
	protected:
		IOverlay() {}
	};

	// Update the overlay and return true if the overlay should be disposed
	bool IOverlay::tickUpdate()
	{
		update();
		return false;
	}
}