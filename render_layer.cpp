#include "weblegends.h"
#include "helpers.h"

#include "df/world.h"
#include "df/world_data.h"
#include "df/world_underground_region.h"

REQUIRE_GLOBAL(world)

void WebLegends::render_layer(std::ostream & s, int32_t id)
{
	CoreSuspender suspend;

	auto layer = df::world_underground_region::find(id);
	if (layer == nullptr)
	{
		return;
	}

	simple_header(s, layer);

	s << "<svg width=\"100%\" style=\"max-width:500px;border:1px solid;float:right\" viewBox=\"0 0 " << world->world_data->world_width << " " << world->world_data->world_height << "\">";
	for (size_t i = 0; i < layer->region_coords.size(); i++)
	{
		s << "<rect width=\"1\" height=\"1\" x=\"" << layer->region_coords.x.at(i) << "\" y=\"" << layer->region_coords.y.at(i) << "\"></rect>";
	}
	s << "</svg>";

	s << "<p>";
	categorize(s, layer);
	s << "</p>";

	history(s, layer);
	// TODO
	s << "</body></html>";
}
