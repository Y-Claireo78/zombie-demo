#ifndef GAMEWORLD_HPP__
#define GAMEWORLD_HPP__

#include <list>
#include <memory>

#include "pvz/Framework/WorldBase.hpp"
#include "pvz/GameObject/GameObject.hpp"
#include "pvz/Framework/TextBase.hpp"
#include "pvz/utils.hpp"

class RedLine;
class ProgressMeter;
class ZombieCardBase;

class GameWorld : public WorldBase
{
public:
  GameWorld() = default;
  ~GameWorld() = default;

  void Init() override;
  LevelStatus Update() override;
  void CleanUp() override;

  // for GameObject
  void AddObject(std::shared_ptr<GameObject> obj);
  bool HasPlant(int row, int col) const;
  bool HasZombieInRow(int row, int minX) const;
  bool HasZombieAt(int row, int col) const;

  // for pvzombie
  bool HasPlantOverlap(int x, int y, int w, int h) const;

  bool SpendSun(int amount);
  void AddSun(int amount) { m_sun += amount; }
  int GetSun() const { return m_sun; }

  int GetCurrentRound() const { return m_currentRound; }
  int GetDeploymentCol() const;
  void EatBrain(int row);

  // cards
  ZombieType GetSelectedCard() const { return m_selectedCard; }
  void SetSelectedCard(ZombieType t) { m_selectedCard = t; }
  void ClearSelectedCard() { m_selectedCard = ZombieType::NONE; }
  void BeginCooldown(ZombieType type);

private:
  std::list<std::shared_ptr<GameObject>> m_objects;
  std::list<std::shared_ptr<GameObject>> m_pendingAdd;

  std::shared_ptr<TextBase> m_sunText;
  std::shared_ptr<TextBase> m_gameOverText;
  std::shared_ptr<RedLine> m_redLine;
  std::shared_ptr<ProgressMeter> m_stageMeter;
  std::shared_ptr<ZombieCardBase> m_regularCard;
  std::shared_ptr<ZombieCardBase> m_pvCard;

  int m_sun = 150;
  int m_currentRound = 1;
  bool m_brainEaten[GAME_ROWS] = {false};
  bool m_plantGrid[GAME_ROWS][GAME_COLS] = {};

  ZombieType m_selectedCard = ZombieType::NONE;

  bool CheckOverlap(std::shared_ptr<GameObject> a, std::shared_ptr<GameObject> b);
  void GeneratePlants();
  void HandleCollisions();
  void ResetRound();
  bool CheckWinLose();
};

#endif // !GAMEWORLD_HPP__