#include "weblegends.h"
#include "helpers.h"

#include "df/world.h"
#include "df/world_data.h"
#include "df/world_site.h"

REQUIRE_GLOBAL(world);

bool WebLegends::render_site(std::ostream & s, int32_t id, int32_t page)
{
    CoreSuspender suspend;

    auto site = df::world_site::find(id);
    if (site == nullptr)
    {
        return false;
    }

    simple_header(s, site);

    s << "<svg width=\"100%\" style=\"max-width:500px;border:1px solid;float:right\" viewBox=\"0 0 " << (world->world_data->world_width * 16) << " " << (world->world_data->world_height * 16) << "\">";
    s << "<rect width=\"" << (site->global_max_x - site->global_min_x + 1) << "\" height=\"" << (site->global_max_y - site->global_min_y + 1) << "\" x=\"" << site->global_min_x << "\" y=\"" << site->global_min_y << "\"></rect>";
    s << "</svg>";

    s << "<p>";
    categorize(s, site);
    s << "</p>";

    if (!site->buildings.empty())
    {
        s << "<h2 id=\"structures\">Structures</h2><ul>";
        for (auto it = site->buildings.begin(); it != site->buildings.end(); it++)
        {
            s << "<li>";
            link(s, *it);
            categorize(s, *it);
            s << "</li>";
        }
        s << "</ul>";
    }
    int32_t last_page;
    if (!history(s, site, page, last_page))
    {
        return false;
    }
    pagination(s, stl_sprintf("site-%d", id), "", "?page=", page, last_page);
    s << "</body></html>";
    return true;
}
