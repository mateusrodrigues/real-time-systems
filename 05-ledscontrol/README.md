## Projeto2.2 - Controle de Leds utilizando prioridade

#### ledscontrol.py - Teste ADC do potenciomêtro
#### renice.py - Versão final do projeto

Implemente 03 processos na Beagle Bone (P1,P2,P3).

- Processo P1, com alta prioridade, lê 02 ADCs a cada segundo. Use esses valores para setar a prioridade de P2 e P3. A prioridade de P1 deve ser sempremaior que a de P2 e P3, para não haver starvation.

- Processos P2 e P3: ficam num loop infinito com a seguinte execução:

    - executa uma carga.

   -  acende o Led respectivo (Led 1 ou Led2), via porta digital.

   - executa uma carga.

   -  apaga o Led respectivo (Led 1 ou Led2), via porta digital.


