#include <internal_node.h>

extern InfoModel mainModel;
extern int ramificationFactor;

//cria um nó interno na MP e retorna um ponteiro pra ele
internalNode *internalNodeCreate()
{
    int maxNumberOfKeys = (ramificationFactor * 2) - 1;
    int maxNumberOfChildren = ((ramificationFactor * 2));

    internalNode *newNode = (internalNode *)malloc(sizeof(internalNode));

    newNode->IDs = (int *)malloc(sizeof(int) * maxNumberOfKeys);
    newNode->children = (Address *)malloc(sizeof(Address) * maxNumberOfChildren);

    newNode = internalNodeInitialize(newNode);

    return newNode;
}

static internalNode *internalNodeInitialize(internalNode *node)
{

    int maxNumberOfKeys = (ramificationFactor * 2) - 1;
    int maxNumberOfChildren = ((ramificationFactor * 2));
    //numero de chaves iniciando com 0
    node->numberOfKeys = 0;
    //todos os ID's sendo setado como 0
    for (int i = 0; i < maxNumberOfKeys; i++)
    {
        node->IDs[i] = 0;
    }
    //todas no vetor de filhos apontando pra -1 (não apontando pra nenhum nó)
    for (int i = 0; i < maxNumberOfChildren; i++)
    {
        node->children[i] = -1;
    }

    return node;
}

/*guarda um nó no arquivo de indices e retorna o endereço onde foi salvo no arquivo.
para inserir no final do arquivo, passe como parâmetro -1 para a Posição de inserção*/
Address internalNodeStore(internalNode *node, Address insertPosition)
{
    int maxNumberOfKeys = (ramificationFactor * 2) - 1;
    int maxNumberOfChildren = ((ramificationFactor * 2));

    FILE *archive = fopen(MAIN_INDEX_FILE_PATH, "rb+");

    if (insertPosition < 0)
    {
        fseek(archive, 0, SEEK_END);
        insertPosition = ftell(archive);
    }
    else
    {
        fseek(archive, insertPosition, SEEK_SET);
    }

    fwrite(node->numberOfKeys, sizeof(int), 1, archive);
    fwrite(node->IDs, sizeof(int), maxNumberOfKeys, archive);
    fwrite(node->children, sizeof(Address), maxNumberOfChildren, archive);

    fclose(archive);

    return insertPosition;
}
//carrega um nó interno pra memória principal e retorna um ponteiro pra ele na MP
internalNode *internalNodeLoad(Address nodePosition)
{

    int maxNumberOfKeys = (ramificationFactor * 2) - 1;
    int maxNumberOfChildren = ((ramificationFactor * 2));

    internalNode *node = internalNodeCreate();
    FILE *archive = fopen(MAIN_INDEX_FILE_PATH, "rb");

    fseek(archive, nodePosition, SEEK_SET);
    fread(node->numberOfKeys, sizeof(int), 1, archive);
    fread(node->IDs, sizeof(int), maxNumberOfKeys, archive);
    fread(node->children, sizeof(Address), maxNumberOfChildren, archive);

    fclose(archive);

    return node;
}
//libera um nó na MP
void internalNodeFree(internalNode *node)
{
    free(node->IDs);
    free(node->children);
    free(node);
}
