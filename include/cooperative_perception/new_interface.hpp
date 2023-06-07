#include <unordered_map>
#include <vector>
#include <string>
#include <variant>

namespace cooperative_perception
{
using ScoreMap = std::unordered_map<std::pair<std::string, std::string>, float>;
using AssociationMap = std::unordered_map<std::string, std::vector<std::string>>;

template <typename Scorer, typename TrackType, typename ObjectType>
auto score(Scorer scorer, const std::vector<TrackType>& tracks, const std::vector<ObjectType>& objects) -> ScoreMap
{
  ScoreMap scores;

  for (const auto& track : tracks)
  {
    const auto track_uuid{ std::visit(uuid_visitor, track) };

    for (const auto& object : objects)
    {
      const auto object_uuid{ std::visit(uuid_visitor, object) };

      scores[{ track_uuid, object_uuid }] = scorer.score(track, object);
    }
  }

  return scores;
}

template <typename Gater>
auto gate(Gater gater, const ScoreMap& scores) -> ScoreMap
{
  ScoreMap gated_scores;

  for (const auto [uuid_pair, score] : scores)
  {
    gated_scores[uuid_pair] = gater.gate(uuid_pair, score);
  }

  return gated_scores;
}

template <typename Associator>
auto associate(Associator associator, const ScoreMap& scores) -> AssociationMap
{
  AssociationMap associations;

  for (const auto& [uuid_pair, score] : scores)
  {
    associations[uuid_pair.first] = associator.associate(uuid_pair.first, scores);
  }

  return associations;
}

template <typename ObjectType>
auto objects_matching_uuids(const std::vector<ObjectType>& objects, const std::vector<std::string>& uuids)
    -> std::vector<ObjectType>
{
  std::vector<ObjectType> matched_objects;

  for (const auto& uuid : uuids)
  {
    matched_objects.emplace_back(*std::find_if(std::cbegin(objects), std::cend(objects), [&uuid](const auto object) {
      return std::visit(uuid_visitor, object) == uuid;
    }));
  }

  return matched_objects;
}

template <typename TrackType, typename Fuser, typename ObjectType>
auto fuse(Fuser fuser, const std::vector<ObjectType>& objects, const AssociationMap& associations)
    -> std::vector<TrackType>
{
  std::vector<TrackType> fused_objects;

  for (const auto [track_uuid, associated_uuids])
  {
    const auto associated_objects{ objects_matching_uuids(objects, associated_uuids) };
    fused_objects.emplace_back(fuser.fuse(track_uuid, associated_objects));
  }

  return fused_objects;
}

template <typename Estimator, typename TrackType>
auto estimate(Estimator estimator, std::vector<TrackType> previous_tracks, std::vector<TrackType> new_tracks)
    -> std::vector<TrackType>
{
  std::vector<TrackType> updated_tracks;

  if (std::size(previous_tracks) != std::size(new_tracks))
  {
    throw std::logic_error("Previous track list size not equal to new track list size.");
  }

  std::transform(std::cbegin(previous_tracks), std::cend(previous_tracks), std::cbegin(new_tracks),
                 std::back_inserter(updated_tracks), [&estimator](const auto& prev_track, const auto& new_track) {
                   return estimator.estimate(prev_track, new_track);
                 });

  return updated_tracks;
}

}  // namespace cooperative_perception

struct EuclideanScorer
{
};

struct FixedRadiusGater
{
};

struct GnnAssociator
{
};

struct CovarianceFuser
{
};

struct UkfEstimator
{
};

struct CtrvTrack
{
};

struct CtrvObject
{
};

using TrackType = std::variant<CtrvTrack>;
using ObjectType = std::variant<CtrvObject>;

void example_usage()
{
  namespace cp = cooperative_perception;

  const std::vector<TrackType> tracks{};
  const std::vector<ObjectType> objects{};

  const auto scores = cp::score(EuclideanScorer(), tracks, objects);
  const auto gated_scores = cp::gate(FixedRadiusGater(), scores);
  const auto associations = cp::associate(GnnAssociator(), gated_scores);
  const auto new_tracks = cp::fuse<TrackType>(CovarianceFuser(), objects, associations);
  const auto updated_tracks = cp::estimate(UkfEstimator(), tracks, new_tracks);
}
