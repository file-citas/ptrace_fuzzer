#include "outputthread.h"
#include "target.h"
#include <QDebug>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdbool.h>

OutputThread::OutputThread(QObject *parent) :
	QThread(parent)
{
}

void OutputThread::process()
{
	char buffer[1024];
	int readp = 0;
	int writep = 0;
	bool eof = false;
	int            max_fd;
	fd_set         input;
	int fd = T::arget().fd();
	//int fd = open("target.out", O_RDONLY, S_IRUSR | S_IWUSR);

	qDebug() << "thread";
	for(;;) {
		/* Initialize the input set */
		FD_ZERO(&input);
		FD_SET(fd, &input);

		max_fd = fd + 1;

		/* Do the select */
		int n = select(max_fd, &input, NULL, NULL, NULL);
		//qDebug() << "HI!";
		/* See if there was an error */
		if (n < 0)
			perror("select failed");
		else if (n == 0)
			puts("TIMEOUT");
		else
		{
			/* We have input */
			if (FD_ISSET(fd, &input)) {
				while (read(fd, buffer, sizeof(buffer)) != 0)
				{
					qDebug() << buffer;
				}
			}
		}
	}

	emit finished();
}
