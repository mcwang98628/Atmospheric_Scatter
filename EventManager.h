#include <map>
#include <string>
#include <vector>
#include <functional>


class EventManager {
	std::map<std::string, std::vector<std::function<void(float)>>> eventMap;
public:
    void on(const std::string& evt_name, std::function<void(float)> listener) {
        auto it = eventMap.find(evt_name);
        if (it != eventMap.end()) {
            it->second.push_back(listener);
            return;
        }
        eventMap[evt_name] = std::vector<std::function<void(float)>>();
        eventMap[evt_name].push_back(listener);
    };
    void emit(const std::string& evt_name, float data) {
        auto evts = eventMap.find(evt_name);
        if (evts == eventMap.end()) return;
        for (std::vector<std::function<void(float)>>::iterator it = std::begin(evts->second); it != std::end(evts->second); ++it) {
            auto& func = *it;
            func(data);
        }
    }
};