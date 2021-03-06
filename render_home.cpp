#include "weblegends.h"
#include "helpers.h"

#include "df/world.h"
#include "df/world_data.h"

REQUIRE_GLOBAL(world);

void WebLegends::render_home(std::ostream & s)
{
    CoreSuspender suspend;

    if (!world->world_data)
    {
        return;
    }

    void simple_header(std::ostream &, df::world_data *);
    simple_header(s, world->world_data);
    s << "<table>";
    s << "<tr><th><a href=\"eras-0\">Eras</a></th><td>" << world->history.eras.size() << "</td></tr>";
    s << "<tr><th><a href=\"figs-0\">Historical Figures</a></th><td>" << world->history.figures.size() << "</td></tr>";
    s << "<tr><th><a href=\"sites-0\">Sites</a></th><td>" << world->world_data->sites.size() << "</td></tr>";
    s << "<tr><th><a href=\"items-0\">Artifacts</a></th><td>" << world->artifacts.all.size() << "</td></tr>";
    s << "<tr><th><a href=\"regions-0\">Regions</a></th><td>" << world->world_data->regions.size() << "</td></tr>";
    s << "<tr><th><a href=\"layers-0\">Underground Regions</a></th><td>" << world->world_data->underground_regions.size() << "</td></tr>";
    s << "<tr><th><a href=\"ents-0\">Civilizations and other entities</a></th><td>" << world->entities.all.size() << "</td></tr>";

    auto handlers = get_handlers_v0();
    if (handlers != nullptr)
    {
        for (auto & h : *handlers)
        {
            if (!h.second.first.empty())
            {
                s << "<tr><th colspan=\"2\"><a href=\"" << h.first << "\">" << h.second.first << "</a></th></tr>";
            }
        }
    }

    s << "</table></body></html>";
}
