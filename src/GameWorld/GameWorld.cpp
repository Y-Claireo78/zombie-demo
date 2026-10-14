#include "pvz/GameWorld/GameWorld.hpp"
#include "UI/UI.hpp"
#include "UI/DeploymentLane.hpp"
#include "UI/Sun.hpp"
#include "Zombies/RegularZombie.hpp"
#include "Zombies/Zombie.hpp"
#include "Zombies/PoleVaultingZombie.hpp"
#include "Plants/Peashooter.hpp"
#include "Plants/Sunflower.hpp"
#include "Plants/WallNut.hpp"
#include "Projectiles/Pea.hpp"
#include "Plants/Repeater.hpp"

void GameWorld::Init()
{
    m_objects.clear();
    m_pendingAdd.clear();
    m_sun = 9999;
    m_currentRound = 5;
    for (int r = 0; r < GAME_ROWS; ++r)
    {
        m_brainEaten[r] = false;
        for (int c = 0; c < GAME_COLS; ++c)
            m_plantGrid[r][c] = false;
    }
    m_selectedCard = ZombieType::NONE;

    auto bg = std::make_shared<Background>();
    bg->SetWorld(this);
    m_objects.push_back(bg);

    m_regularCard = std::make_shared<ZombieCard>(ZOMBIE_CARD_FIRST_X, ZOMBIE_CARD_Y);
    m_regularCard->SetWorld(this);
    m_objects.push_back(m_regularCard);

    m_pvCard = std::make_shared<PoleVaultingCard>(
        ZOMBIE_CARD_FIRST_X + ZOMBIE_CARD_SPACING, ZOMBIE_CARD_Y);
    m_pvCard->SetWorld(this);
    m_objects.push_back(m_pvCard);

    auto regularMask = std::make_shared<CooldownMask>(
        ZOMBIE_CARD_FIRST_X, ZOMBIE_CARD_Y, m_regularCard.get());
    regularMask->SetWorld(this);
    m_objects.push_back(regularMask);

    auto pvMask = std::make_shared<CooldownMask>(
        ZOMBIE_CARD_FIRST_X + ZOMBIE_CARD_SPACING, ZOMBIE_CARD_Y, m_pvCard.get());
    pvMask->SetWorld(this);
    m_objects.push_back(pvMask);

    // brain
    for (int row = 0; row < GAME_ROWS; ++row)
    {
        auto brain = std::make_shared<Brain>(row);
        brain->SetWorld(this);
        m_objects.push_back(brain);
    }

    // redline
    int redX = LAWN_GRID_LEFT +
               (FINAL_ZOMBIE_DEPLOYMENT_START_COL+ZOMBIE_DEPLOYMENT_BUFFER_COLS) * LAWN_GRID_WIDTH;
    m_redLine = std::make_shared<RedLine>(redX);
    m_redLine->SetWorld(this);
    m_objects.push_back(m_redLine);

    // jindutiao
    int meterY = 50;
    auto empty = std::make_shared<ProgressMeter>(WINDOW_WIDTH / 2, meterY,
                                                 ImageID::PROGRESS_METER_EMPTY);
    empty->SetWorld(this);
    m_objects.push_back(empty);
    m_stageMeter = std::make_shared<ProgressMeter>(WINDOW_WIDTH / 2, meterY + 10,
                                                   ImageID::PROGRESS_METER_STAGE_1);
    m_stageMeter->SetWorld(this);
    m_objects.push_back(m_stageMeter);

    //
    for (int row = 0; row < GAME_ROWS; ++row)
    {
        for (int col = 0; col < GAME_COLS; ++col)
        {
            auto lane = std::make_shared<DeploymentLane>(row, col);
            lane->SetWorld(this);
            m_objects.push_back(lane);
        }
    }

    GeneratePlants();

    m_sunText = std::make_shared<TextBase>(SUN_COUNTER_X, SUN_COUNTER_Y);
    m_sunText->SetText(std::to_string(m_sun));

    m_gameOverText = std::make_shared<TextBase>(
        WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "", 48, 255, 0, true);
}

LevelStatus GameWorld::Update()
{
    for (auto &obj : m_objects)
        obj->Update();

    if (!m_pendingAdd.empty())
    {
        for (auto &obj : m_pendingAdd)
        {
            obj->SetWorld(this);
            m_objects.push_back(obj);
        }
        m_pendingAdd.clear();
    }

    HandleCollisions();

    auto it = m_objects.begin();
    while (it != m_objects.end())
    {
        if ((*it)->IsDead())
        {
            if ((*it)->IsPlant())
            {
                int r = (*it)->GetRow();
                int c = (*it)->GetCol();
                if (r >= 0 && r < GAME_ROWS && c >= 0 && c < GAME_COLS)
                    m_plantGrid[r][c] = false;
            }
            it = m_objects.erase(it);
        }
        else
        {
            ++it;
        }
    }

    m_sunText->SetText(std::to_string(m_sun));

    bool allEaten = true;
    for (int r = 0; r < GAME_ROWS; ++r)
    {
        if (!m_brainEaten[r])
        {
            allEaten = false;
            break;
        }
    }
    if (allEaten)
    {
        if (m_currentRound == TOTAL_ROUNDS)
            return LevelStatus::WINNING;
        ++m_currentRound;
        ResetRound();
    }

    if (CheckWinLose())
    {
        if (m_gameOverText)
            m_gameOverText->SetText("GAME OVER");
        return LevelStatus::LOSING;
    }

    return LevelStatus::ONGOING;
}

void GameWorld::CleanUp()
{
    m_objects.clear();
    m_pendingAdd.clear();
    m_sunText.reset();
    m_gameOverText.reset();
    m_redLine.reset();
    m_stageMeter.reset();
    m_regularCard.reset();
    m_pvCard.reset();
}

void GameWorld::AddObject(std::shared_ptr<GameObject> obj)
{
    m_pendingAdd.push_back(obj);
}

bool GameWorld::HasPlant(int row, int col) const
{
    if (row < 0 || row >= GAME_ROWS || col < 0 || col >= GAME_COLS)
        return false;
    return m_plantGrid[row][col];
}

bool GameWorld::HasZombieInRow(int row, int minX) const
{
    for (const auto &obj : m_objects)
    {
        if (!obj->IsZombie() || obj->IsDead())
            continue;
        int rowCenter = FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT;
        if (std::abs(obj->GetY() - rowCenter) < LAWN_GRID_HEIGHT / 2 &&
            obj->GetX() > minX)
            return true;
    }
    return false;
}

bool GameWorld::HasZombieAt(int row, int col) const
{
    int cx = LAWN_GRID_LEFT + col * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2;
    int cy = FIRST_ROW_CENTER + row * LAWN_GRID_HEIGHT;
    for (const auto &obj : m_objects)
    {
        if (!obj->IsZombie() || obj->IsDead())
            continue;
        if (std::abs(obj->GetX() - cx) < LAWN_GRID_WIDTH / 2 &&
            std::abs(obj->GetY() - cy) < LAWN_GRID_HEIGHT / 2)
            return true;
    }
    return false;
}

bool GameWorld::HasPlantOverlap(int x, int y, int w, int h) const
{
    int aw = w / 2, ah = h / 2;
    for (const auto &obj : m_objects)
    {
        if (!obj->IsPlant() || obj->IsDead())
            continue;
        int bw = obj->GetWidth() / 2, bh = obj->GetHeight() / 2;
        if (x - aw < obj->GetX() + bw && x + aw > obj->GetX() - bw &&
            y - ah < obj->GetY() + bh && y + ah > obj->GetY() - bh)
            return true;
    }
    return false;
}

bool GameWorld::SpendSun(int amount)
{
    if (m_sun >= amount)
    {
        m_sun -= amount;
        return true;
    }
    return false;
}

int GameWorld::GetDeploymentCol() const
{
    return INITIAL_ZOMBIE_DEPLOYMENT_START_COL + ZOMBIE_DEPLOYMENT_BUFFER_COLS + (m_currentRound - 1);
}

void GameWorld::EatBrain(int row)
{
    if (row >= 0 && row < GAME_ROWS)
        m_brainEaten[row] = true;
}

void GameWorld::BeginCooldown(ZombieType type)
{
    if (type == ZombieType::REGULAR && m_regularCard)
        m_regularCard->StartCooldown();
    else if (type == ZombieType::POLEVAULTING && m_pvCard)
        m_pvCard->StartCooldown();
}

bool GameWorld::CheckOverlap(std::shared_ptr<GameObject> a,
                             std::shared_ptr<GameObject> b)
{
    int ax = a->GetX(), ay = a->GetY();
    int aw = a->GetWidth() / 2, ah = a->GetHeight() / 2;
    int bx = b->GetX(), by = b->GetY();
    int bw = b->GetWidth() / 2, bh = b->GetHeight() / 2;
    return (ax - aw < bx + bw && ax + aw > bx - bw &&
            ay - ah < by + bh && ay + ah > by - bh);
}

void GameWorld::GeneratePlants()
{
    int availableCols = GetDeploymentCol();
    if (availableCols > GAME_COLS)
        availableCols = GAME_COLS;
    if (availableCols < 2)
        availableCols = 2;

    const int basePlants = GAME_ROWS * 2;
    const int maxPlants = 18;
    int totalPlants = basePlants;
    if (m_currentRound > 2)
    {
        int extraMax = maxPlants - basePlants;
        int span = TOTAL_ROUNDS - 2;
        totalPlants = basePlants + extraMax * (m_currentRound - 2) / span;
    }

    std::vector<int> rowCounts(GAME_ROWS, 2);
    int extra = totalPlants - basePlants;
    while (extra > 0)
    {
        int r = randInt(0, GAME_ROWS - 1);
        if (rowCounts[r] < availableCols)
        {
            ++rowCounts[r];
            --extra;
        }
    }

    int wallNutRow = randInt(0, GAME_ROWS - 1);

    for (int row = 0; row < GAME_ROWS; ++row)
    {
        std::vector<int> cols;
        for (int c = 0; c < availableCols; ++c)
            cols.push_back(c);
        for (int i = static_cast<int>(cols.size()) - 1; i > 0; --i)
        {
            int j = randInt(0, i);
            int tmp = cols[i];
            cols[i] = cols[j];
            cols[j] = tmp;
        }
        cols.resize(rowCounts[row]);

        if (m_currentRound >= 3 && row % 2 == 1)
            AddObject(std::make_shared<Repeater>(row, cols[0]));
        else
            AddObject(std::make_shared<Peashooter>(row, cols[0]));
        m_plantGrid[row][cols[0]] = true;

        if (row == wallNutRow)
            AddObject(std::make_shared<WallNut>(row, cols[1]));
        else
            AddObject(std::make_shared<Sunflower>(row, cols[1]));
        m_plantGrid[row][cols[1]] = true;

        for (std::size_t i = 2; i < cols.size(); ++i)
        {
            if (randInt(0, 1) == 0)
                AddObject(std::make_shared<WallNut>(row, cols[i]));
            else
                AddObject(std::make_shared<Sunflower>(row, cols[i]));
            m_plantGrid[row][cols[i]] = true;
        }
    }
}

void GameWorld::HandleCollisions()
{
    for (auto &pea : m_objects)
    {
        if (!pea->IsPea() || pea->IsDead())
            continue;
        for (auto &zombie : m_objects)
        {
            if (!zombie->IsZombie() || zombie->IsDead())
                continue;
            if (CheckOverlap(pea, zombie))
            {
                zombie->TakeDamage(24);
                pea->SetDead();
                break;
            }
        }
    }

    for (auto &zombieObj : m_objects)
    {
        if (!zombieObj->IsZombie() || zombieObj->IsDead())
            continue;
        bool hasTarget = false;
        for (auto &plantObj : m_objects)
        {
            if (!plantObj->IsPlant() || plantObj->IsDead())
                continue;
            if (CheckOverlap(zombieObj, plantObj))
            {
                zombieObj->SetTarget(plantObj);
                hasTarget = true;
                break;
            }
        }
        if (!hasTarget)
            zombieObj->SetTarget(nullptr);
    }

    for (auto &zombieObj : m_objects)
    {
        if (!zombieObj->IsZombie() || zombieObj->IsDead())
            continue;
        for (auto &brainObj : m_objects)
        {
            if (!brainObj->IsBrain() || brainObj->IsDead())
                continue;
            if (CheckOverlap(zombieObj, brainObj))
            {
                brainObj->SetDead();
                EatBrain(brainObj->GetRow());
                break;
            }
        }
    }
}

void GameWorld::ResetRound()
{
    auto it = m_objects.begin();
    while (it != m_objects.end())
    {
        auto &obj = *it;
        if (obj->IsPlant() || obj->IsZombie() || obj->IsPea() ||
            obj->IsSun() || obj->IsBrain())
            it = m_objects.erase(it);
        else
            ++it;
    }

    for (int r = 0; r < GAME_ROWS; ++r)
    {
        m_brainEaten[r] = false;
        auto brain = std::make_shared<Brain>(r);
        brain->SetWorld(this);
        m_objects.push_back(brain);
    }

    for (int r = 0; r < GAME_ROWS; ++r)
        for (int c = 0; c < GAME_COLS; ++c)
            m_plantGrid[r][c] = false;

    GeneratePlants();

    int newRedX = LAWN_GRID_LEFT +
                  (INITIAL_ZOMBIE_DEPLOYMENT_START_COL + ZOMBIE_DEPLOYMENT_BUFFER_COLS + (m_currentRound - 1)) * LAWN_GRID_WIDTH;
    m_redLine->MoveTo(newRedX, m_redLine->GetY());

    ImageID stageImg = static_cast<ImageID>(
        static_cast<int>(ImageID::PROGRESS_METER_STAGE_1) + (m_currentRound - 1));
    m_stageMeter->ChangeImage(stageImg);

    m_sun = 150;
    m_sunText->SetText(std::to_string(m_sun));
    m_selectedCard = ZombieType::NONE;
}

bool GameWorld::CheckWinLose()
{
    bool anyBrainLeft = false;
    for (int r = 0; r < GAME_ROWS; ++r)
        if (!m_brainEaten[r])
        {
            anyBrainLeft = true;
            break;
        }
    if (!anyBrainLeft)
        return false;

    if (m_sun >= ZOMBIE_PRICE_REGULAR)
        return false;

    for (const auto &obj : m_objects)
        if (obj->IsSun() && !obj->IsDead())
            return false;

    for (const auto &obj : m_objects)
        if (obj->IsZombie() && !obj->IsDead())
            return false;

    return true;
}