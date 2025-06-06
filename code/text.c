#include "raylib.h"

// meu primeiro "projeto" testando a raylib de verdade pela primeira vez e tentando fazer alguma coisa <3 
int main()
{
    SetTargetFPS(60); // o proprio nome já diz

    int x = 400;
    int y = 300; // "criando" o personagem nas coordenadas x e y;
    bool olhandoEsquerda = false;
    int vida = 100;

    float velocidadeY = 0;
    float gravidade = 0.5f;    
    int movimento = 5; // deslocamento do personagem

    InitWindow(800, 600, "SLIME HERO"); // parametros dessa função é receber a altura e depois a largura da janela a ser inicializada

    Texture2D slime = LoadTexture("assets/Slime_Blue.png"); // sprite sheet com 4 quadros de 32x32 pixels

    Rectangle frameRec = { 0.0f, 0.0f, 32, 32 }; // define qual parte da imagem será desenhada
    Vector2 posicao = { x, y };

    // variaveis de animação
    int frameAtual = 0;
    float tempoAcumulado = 0;
    float velocidadeAnimacao = 0.2f; // quanto menor, mais rápido

    int estadoJogo = 0; // 0: Tela início, 1: Jogando, 2: Game Over

    // Variáveis para animar a morte
    bool morrendo = false;
    float tempoMorteTotal = 0.0f;
    float duracaoMorte = 1.6f; // 8 frames * 0.2s

    while (!WindowShouldClose()) // laço principal do projeto onde a janela continuará aberta até que seja fechada
    {
        BeginDrawing(); // função para "desenhar" na tela, não recebe parametros
        ClearBackground(PURPLE); // função para limpar a tela de fundo e receber uma cor como parametro

        if (estadoJogo == 0)
        {
            // Tela de início
            DrawText("Bem-vindo ao Slime Hero!", 220, 250, 30, WHITE);
            DrawText("Pressione ENTER para comecar", 220, 280, 20, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                estadoJogo = 1;
            }
        }
        else if (estadoJogo == 1)
        {
            // Atualização da posição (movimentação)
            if (!morrendo)
            {
                if (IsKeyDown(KEY_D) && x + 34 < 800)
                {
                    x += movimento;
                    olhandoEsquerda = true;
                }

                if (IsKeyDown(KEY_A) && x > 0)
                {
                    x -= movimento;
                    olhandoEsquerda = false;
                }

                if (IsKeyDown(KEY_S) && y + frameRec.height < 600)
                {
                    y += movimento;
                }
            }

            posicao.x = x;
            velocidadeY += gravidade;
            posicao.y += velocidadeY;

            // Colisão com "chão"
            if (posicao.y >= 500)
            {
                posicao.y = 500;
                velocidadeY = 0;

                // pulo 
                if (!morrendo && IsKeyPressed(KEY_W))
                {
                    velocidadeY = -10;              
                }
            }

            // lógica de animação
            float delta = GetFrameTime();
            tempoAcumulado += delta;

            if (morrendo)
            {
                // Linha 3 da sprite sheet = animação de morte
                frameRec.y = 3 * 32;

                if (tempoAcumulado >= velocidadeAnimacao)
                {
                    frameAtual++;
                    tempoAcumulado = 0;
                    if (frameAtual > 7) frameAtual = 7; // para no último frame
                }

                tempoMorteTotal += delta;
                if (tempoMorteTotal >= duracaoMorte)
                {
                    morrendo = false;
                    estadoJogo = 2; // vai pra tela de Game Over depois da morte
                }
            }
            else
            {
                // Linha 0 da sprite sheet = animação normal (parado ou andando)
                frameRec.y = 0;
                if (tempoAcumulado >= velocidadeAnimacao)
                {
                    frameAtual++;
                    tempoAcumulado = 0;
                    if (frameAtual > 5) frameAtual = 0; // voltar pro primeiro quadro
                }
            }

            // Atualização da posição do frame atual na horizontal (x)
            frameRec.x = frameAtual * 32;
            frameRec.width = olhandoEsquerda ? -32 : 32;

            // UI de vida
            DrawText(TextFormat("Vida atual: %d", vida), 10, 45, 20, BLACK);  
            DrawRectangle(10, 70, 100 * 2, 20, RED);
            DrawRectangle(10, 70, vida * 2, 20, GREEN);

            // Simulação de dano
            if (IsKeyPressed(KEY_L))
            {
                vida -= 10;

                // Inicia a animação de morte
                if (vida <= 0 && !morrendo)
                {
                    morrendo = true;
                    tempoMorteTotal = 0.0f;
                    tempoAcumulado = 0;
                    frameAtual = 0;
                }
            }

            DrawRectangle(0, 530, 800, 200, GREEN); // chão       

            // Desenhar o slime (tanto vivo quanto morrendo)
            DrawTextureRec(slime, frameRec, posicao, WHITE); // desenha apenas o quadro da sprite sheet na posição
        }
        else if (estadoJogo == 2)
        {
            // Tela de Game Over
            DrawText("Game Over!", 250, 200, 50, RED);
            DrawText("Pressione R para reiniciar", 250, 300, 20, WHITE);

            if (IsKeyPressed(KEY_R))
            {
                // Reinicia o jogo
                estadoJogo = 1;
                vida = 100;
                x = 400;
                y = 300;
                velocidadeY = 0;
                posicao.x = x;
                posicao.y = y;
                frameAtual = 0;
                tempoAcumulado = 0;
                morrendo = false;
                tempoMorteTotal = 0;
            }
        }

        EndDrawing(); // finaliza o processo de desenho
    }

    // liberação de recursos
    UnloadTexture(slime); // liberar memoria da textura
    CloseWindow(); // fecha a janela

    return 0;
}
