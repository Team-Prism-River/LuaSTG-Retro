#pragma once
#include <algorithm>
#include "core/Vector2.hpp"
#include "core/Rational.hpp"
#include "core/ReferenceCounted.hpp"
#include "Core/Graphics/Window.hpp"
#include "Core/Graphics/Format.hpp"
#include "Core/Graphics/Device.hpp"

namespace core::Graphics
{
	struct DisplayMode
	{
		uint32_t width{ 0 };
		uint32_t height{ 0 };
		Rational refresh_rate;
		Format format{ Format::B8G8R8A8_UNORM };
	};

	enum class SwapChainScalingMode
	{
		Stretch,
		AspectRatio,
		IntegerAspectRatio,
	};

	struct SwapChainPresentationLayout
	{
		Vector2F offset;
		Vector2F size;
		Vector2F scale;
		bool use_point_filter{ false };
	};

	inline SwapChainPresentationLayout makeSwapChainPresentationLayout(Vector2U outer_size, Vector2U inner_size, SwapChainScalingMode mode) noexcept
	{
		SwapChainPresentationLayout layout;

		if (outer_size.x == 0 || outer_size.y == 0 || inner_size.x == 0 || inner_size.y == 0)
		{
			return layout;
		}

		if (mode == SwapChainScalingMode::Stretch)
		{
			layout.offset = Vector2F(0.0f, 0.0f);
			layout.size = Vector2F(static_cast<float>(outer_size.x), static_cast<float>(outer_size.y));
			layout.scale = Vector2F(
				static_cast<float>(static_cast<double>(outer_size.x) / static_cast<double>(inner_size.x)),
				static_cast<float>(static_cast<double>(outer_size.y) / static_cast<double>(inner_size.y)));
			return layout;
		}

		double scale = std::min(
			static_cast<double>(outer_size.x) / static_cast<double>(inner_size.x),
			static_cast<double>(outer_size.y) / static_cast<double>(inner_size.y));
		if (mode == SwapChainScalingMode::IntegerAspectRatio && scale >= 1.0)
		{
			scale = static_cast<double>(static_cast<uint32_t>(scale));
			layout.use_point_filter = true;
		}

		double const width = static_cast<double>(inner_size.x) * scale;
		double const height = static_cast<double>(inner_size.y) * scale;
		layout.offset = Vector2F(
			static_cast<float>((static_cast<double>(outer_size.x) - width) * 0.5),
			static_cast<float>((static_cast<double>(outer_size.y) - height) * 0.5));
		layout.size = Vector2F(static_cast<float>(width), static_cast<float>(height));
		layout.scale = Vector2F(static_cast<float>(scale), static_cast<float>(scale));
		return layout;
	}

	struct ISwapChainEventListener
	{
		virtual void onSwapChainCreate() = 0;
		virtual void onSwapChainDestroy() = 0;
	};

	struct ISwapChain : public IReferenceCounted
	{
		virtual void addEventListener(ISwapChainEventListener* e) = 0;
		virtual void removeEventListener(ISwapChainEventListener* e) = 0;

		virtual bool setWindowMode(Vector2U size) = 0;
		virtual bool setCanvasSize(Vector2U size) = 0;
		virtual Vector2U getCanvasSize() = 0;

		virtual void setScalingMode(SwapChainScalingMode mode) = 0;
		virtual SwapChainScalingMode getScalingMode() = 0;

		virtual void clearRenderAttachment() = 0;
		virtual void applyRenderAttachment() = 0;
		virtual void waitFrameLatency() = 0;
		virtual void setVSync(bool enable) = 0;
		virtual bool getVSync() = 0;
		virtual bool present() = 0;

		virtual bool saveSnapshotToFile(StringView path) = 0;

		static bool create(IWindow* p_window, IDevice* p_device, ISwapChain** pp_swapchain);
	};
}

namespace core {
	// UUID v5
	// ns:URL
	// https://www.luastg-sub.com/core.ISwapChain
	template<> constexpr InterfaceId getInterfaceId<Graphics::ISwapChain>() { return UUID::parse("9036abca-4134-5258-9021-a79b7bfe5a58"); }
}
