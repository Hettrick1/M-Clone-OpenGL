#include "ChunkHandler.h"

#include <unordered_set>
#include <algorithm>

struct ChunkHash { // structure to create a hash of a chunk regarding its position
	size_t operator()(const std::pair<int, int>& pos) const {
		return std::hash<int>()(pos.first) ^ (std::hash<int>()(pos.second) << 1);
	}
};

struct ChunkEqual { // now if two chunk are the same one
	bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
		return a.first == b.first && a.second == b.second;
	}
};

std::unordered_set<std::pair<int, int>, ChunkHash, ChunkEqual> activeChunks;
std::unordered_set<std::pair<int, int>, ChunkHash, ChunkEqual> unactiveChunks;

ChunkHandler::ChunkHandler(int renderDistance, Camera* cam, int seed)
{
	mRenderDistance = renderDistance;
	mCamera = cam;
	mActiveChunks.reserve((renderDistance * 2) * (renderDistance * 2));
    mPreloadChunkFactor = 1;
    mRectWidth = 16;
    mSeed = seed;
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // Texture parameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load the texture of the blocks
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Game/Resources/368x16_sheet.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        mBlockSize = height;
        mTextureWidth = width;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Erreur : " << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
	GenerateAllChunks();
}

ChunkHandler::~ChunkHandler()
{
	for (Chunk* chunk : mActiveChunks) {
		delete chunk;
	}
	mActiveChunks.clear();
	activeChunks.clear();
    mOldChunks.clear();
    unactiveChunks.clear();
}

void ChunkHandler::GenerateAllChunks()
{
    // generate chunks regarding render distance, we can set a preload option to optimize the early game
	for (int i = -mRenderDistance * mPreloadChunkFactor; i < mRenderDistance * mPreloadChunkFactor; i++) {
		for (int j = -mRenderDistance * mPreloadChunkFactor; j < mRenderDistance * mPreloadChunkFactor; j++) {
			std::pair<int, int> chunkPosition = { i, j };
            // if the chunk is in the render distance
			if (activeChunks.find(chunkPosition) == activeChunks.end() && (abs(i) <= mRenderDistance && abs(j) <= mRenderDistance)) {
				activeChunks.insert(chunkPosition);
				mActiveChunks.push_back(new Chunk(mCamera, glm::vec3(i, j, 0), mSeed, mTexture, mTextureWidth, mBlockSize));
			}
            // Add the chunk into the unactive chunk if we preload the chunk and the chunk is outside the render distance
            else if (unactiveChunks.find(chunkPosition) == unactiveChunks.end()){
                unactiveChunks.insert(chunkPosition);
                mOldChunks.push_back(new Chunk(mCamera, glm::vec3(i, j, 0), mSeed, mTexture, mTextureWidth, mBlockSize));
            }
		}
	}
}

void ChunkHandler::UpdateChunks()
{
    // where is the camera in chunk coordinates
    int cameraChunkX = static_cast<int>(std::floor(mCamera->GetPosition().x / 16));
    int cameraChunkY = static_cast<int>(std::floor(mCamera->GetPosition().y / 16));

    glm::vec3 currentPosition = mCamera->GetPosition();
    glm::vec3 direction = currentPosition - mPreviousCameraPosition;
    mPreviousCameraPosition = currentPosition;

    if (glm::length(direction) < 0.01f) { // stops if the player doesn't move
        return;
    }

    // direction angle calcul
    direction = glm::normalize(direction);
    float angle = atan2(direction.y, direction.x);

     // width of the rectangle
    int height = mRenderDistance*2;

    // Chunk generation regarding the position
    for (int i = -height / 2; i <= height / 2; ++i) {
        for (int j = -mRectWidth / 2; j <= mRectWidth / 2; ++j) {
            // Calcul de la position selon l'angle de la direction
            int offsetX = static_cast<int>(std::round(i * cos(angle) - j * sin(angle)));
            int offsetY = static_cast<int>(std::round(i * sin(angle) + j * cos(angle)));

            int chunkX = cameraChunkX + offsetX;
            int chunkY = cameraChunkY + offsetY;

            GenerateNewChunk(chunkX, chunkY);
        }
    }

    // Delete old chunks
    RemoveOldChunk(cameraChunkX, cameraChunkY);
    
}

void ChunkHandler::DrawChunks()
{
	for (int i = 0; i < mActiveChunks.size(); i++) {
		mActiveChunks[i]->Draw();
	}
    for (int i = 0; i < mActiveChunks.size(); i++) {
        // Get the chunk position
        glm::vec3 chunkPos = mActiveChunks[i]->GetPosition();

        // Distance between the chunk and the camera
        float distance = glm::length(chunkPos - glm::vec3(mCamera->GetPosition().x, mCamera->GetPosition().y, 0));

        distance /= 16;

        // This is the folliage render distance, if the distance between the chunk and the camera is smaller than 8 we draw the folliage
        if (distance < 8) {
            mActiveChunks[i]->DrawTransparent();
        }
    }
}

void ChunkHandler::GenerateNewChunk(int chunkX, int chunkY)
{
    std::pair<int, int> newChunkPosition = { chunkX, chunkY };

    // does NOT the chunk already exist
    if (activeChunks.find(newChunkPosition) == activeChunks.end() && unactiveChunks.find(newChunkPosition) == unactiveChunks.end()) {
        // Create new chunk
        Chunk* newChunk = nullptr;
        newChunk = new Chunk(mCamera, glm::vec3(chunkX, chunkY, 0), mSeed, mTexture, mTextureWidth, mBlockSize);
        activeChunks.insert(newChunkPosition);
        mActiveChunks.push_back(newChunk);
    }
    // if the chunk already exists we just retreve is from the oldChunk vector
    else if (unactiveChunks.find(newChunkPosition) != unactiveChunks.end()) {
        auto it = std::find_if(mOldChunks.begin(), mOldChunks.end(),
            [chunkX, chunkY](Chunk* chunk) {
                glm::vec3 pos = chunk->GetPosition();
                return static_cast<int>(pos.x / 16) == chunkX && static_cast<int>(pos.y / 16) == chunkY;
            });

        if (it != mOldChunks.end()) {
            activeChunks.insert(newChunkPosition);
            unactiveChunks.erase(newChunkPosition);
            mActiveChunks.push_back(*it);
            mOldChunks.erase(it);
        }
    }
}

void ChunkHandler::RemoveOldChunk(int cameraChunkX, int cameraChunkY)
{
    // unload the chunks if the camera is too far away
    // still need to delete them from the memory when we are really far away from them
    for (auto it = mActiveChunks.begin(); it != mActiveChunks.end();) {
        glm::vec3 pos = (*it)->GetPosition();
        int posChunkX = static_cast<int>(pos.x / 16);
        int posChunkY = static_cast<int>(pos.y / 16);

        int distX = std::abs(cameraChunkX - posChunkX);
        int distY = std::abs(cameraChunkY - posChunkY);

        if (distX > mRenderDistance || distY > mRenderDistance) {
            activeChunks.erase({ posChunkX, posChunkY });
            unactiveChunks.insert({ posChunkX, posChunkY });  // Add in the unactive chunk list
            mOldChunks.push_back(*it);
            it = mActiveChunks.erase(it);
        }
        else {
            ++it;
        }
    }
}
