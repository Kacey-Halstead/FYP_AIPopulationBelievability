#### ![agent](FYP_AIBelievability/FYP_AIBelievability/Images/AGENT2.png) Simulating Personality and Emotion in AI Agents through Individual-Based Modelling ![agent](FYP_AIBelievability/FYP_AIBelievability/Images/AGENT2.png)

##### Read the full breakdown of this project [here](https://kaceyhalstead.wordpress.com/2025/02/02/simulating-personality-and-emotion-in-ai-agents-through-individual-based-modelling/).

##### Play the simulation on itch.io [here](https://kaceyhalstead.itch.io/simulating-personality-and-emotion-in-ai-agents-through-individual-based-modelli).

![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BLUEBUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BLUEBUSHF.png)
![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BLUEBUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BLUEBUSHF.png)
![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BLUEBUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BUSHF.png) ![bush](FYP_AIBelievability/FYP_AIBelievability/Images/BLUEBUSHF.png)

This simulation consists of 2D agents navigating a generated environment, trying to fulfil their needs and make decisions based on their emotions and personality. 

This project includes the following features:
- Wave Function Collapse
- A* pathfinding
- Goal-Oriented Action Planning system
- Directed Acyclic Graph
- MARPO Reactive Stack
- Utility Theory
- Dear ImGui and ImPlot Display

Every agent has 6 randomly assigned traits, which make up their personality (based on the OCEAN model), which act as modifiers for the agent's 8 emotions (based on Plutchik's model). This means that some agents may feel certain emotions much more strongly than others.
The effects of these traits and personality values can be seen here:

| OCEAN Factors  | High Score Traits | Low Score Traits  | Effect on Emotions with High Score trait |
| -------------- | ----------------- | ----------------- | ------------------ |
| Openness  | Creative, Curious, Complex  | Uncreative, Conventional, Narrow-minded  | -Surprise  +Trust  +Joy  -Fear -Anticipation -Sadness -Disgust  | 
| Conscientiousness  | Reliable, Careful, Self-disciplined  | Disorganised, Undependable, Negligent  | +Sadness -Anger +Anticipation  |
| Extraversion  | Sociable, Friendly, Talkative  | Introverted, Quiet, Reserved  | +Trust +Joy +Sadness -Anticipation  | 
| Agreeableness  | Sympathetic, Forgiving, Courteous  | Critical, Rude, Harsh  | +Joy +Surprise +Trust -Anticipation -Disgust  |
| Neuroticism  | Nervous, Insecure, Worrying  | Calm, Relaxed, Secure  | +Anger +Surprise -Trust +Disgust +Fear -Joy +Anticipation +Sadness  | 

The high score traits shown above inflict the emotion modifiers in the last column. If an agent gets a low score trait, the opposite effects will be applied. Also, if an agent happens to get multiple traits from the same category, the effects will be applied multiple times.

Every agent has a hunger, thirst and social need. If a need becomes too low, it will be prioritised and this will be the next goal action to complete.

Each agent can be clicked on to view their needs, food preference, personality values, emotion values, and recently completed actions.
