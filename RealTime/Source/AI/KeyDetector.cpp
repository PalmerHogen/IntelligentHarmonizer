#include "KeyDetector.h"
#include <unordered_map>

KeyDetector::KeyDetector(int memorySize)
{
    Updated = false;
    MemorySize = memorySize;
}

KeyDetector::~KeyDetector(){}

void KeyDetector::CatalogChord(Chord chord)
{
    StoredProgression.Add(chord);
    if(StoredProgression.Length() > MemorySize)
        StoredProgression.RemoveFirst();
    Updated = false;
}

Key KeyDetector::DetectKey()
{
    if(!Updated)
    {
        QualifyKey();
        Updated = true;
    }
    return MostLikelyKey;
}

void KeyDetector::SetMemory(Progression &newMemory)
{
    StoredProgression = newMemory;
}

void KeyDetector::QualifyKey()
{
    Key detectedKey;
    int maxWeight = 0;
    unordered_map<Key, int> KeyWeights;
    for(Chord chord : StoredProgression)
    {
        for(int keyType = NaturalMinorKey; keyType <= MajorKey; keyType++)
        {
            KeyPattern pattern((KeyType) keyType);
            
            for(int scaleDegree = 0; scaleDegree < 8; scaleDegree++)
            {
                Key possibleKey = Key(chord.Root - pattern.Intervals[scaleDegree], (KeyType)keyType);
                
                if(!possibleKey.ContainsChord(chord))
                    continue;
                
                if(++KeyWeights[possibleKey] > maxWeight)
                {
                    detectedKey = possibleKey;
                    maxWeight = KeyWeights[possibleKey];
                }
            }
        }
    }
    MostLikelyKey = detectedKey;
}
